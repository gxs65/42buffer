/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crosspoint_rendering.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:40:18 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 20:47:08 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Gives the contribution from global illumination at some point,
// approximated by sending a new "diffusion" ray
// in a random direction on the hemisphere
// /!\ random ray direction is skewed towards normal
// 		(we take a random vector on the unit sphere,
// 		 then add the normal to it and renormalize it)
// /!\ infinitesimal check to avoid having normal_vec and random_unit_vector
// 	   canceling one another and ending with a null diffusion vector
t_vec	ft_color_from_global_illum(t_data *data, t_intersection *intersect)
{
	t_vec	col;
	t_vec	diffusion_vec;
	t_ray	diffusion_ray;
	t_vec	random_unit_vec;

	random_unit_vec = ft_vec_init_random_unit();
	diffusion_vec = ft_vec_add(intersect->normal, random_unit_vec);
	if (ft_vec_is_infinitesimal(diffusion_vec))
		diffusion_vec = ft_vec_copy(intersect->normal);
	diffusion_vec = ft_vec_setnorm(diffusion_vec, 1);
	diffusion_ray = ft_ray_init_by_dir(intersect->crosspoint, diffusion_vec);
	data->recurr_lvl++;
	col = ft_draw_one_ray(data, diffusion_ray);
	data->recurr_lvl--;
	return (col);
}

// Determines color at a point of a lambertian object
// Lambertian surfaces diffuse the light they receive in all directions,
// with light intensity weighted by the angle between angle and surface normal
// 		(ie. more light in the direction of the surface normal)
// but here we add a specular term for the Phong model
// 		(-> it does not behave strictly like a lambertian material)
// So the function simply sums up light received from everywhere
// 		- Phong model : for each visible light in the scene,
// 			surface receives light that is partly
// 				~ diffused in all directions (diffuse)
// 				~ diffused in the symmetric direction (specular)
// 		- ambient lighting (unrealistic) : surface emits a small amount
// 		  of light on its own
// 		- global illumination : surface receives diffused light
// 		  from each other scene object, but we can't compute this,
// 			so we approximate by adding the full light from a ray
// 			sent in a random direction weighted towards normal
t_vec	ft_color_ray_at_object_lambertian(t_data *data, t_ray ray,
	t_intersection *intersect)
{
	t_vec	col;

	if (g_log & B_COLORING)
		printf("[COLORING - LAMBERTIAN] begin coloring\n");
	col = ft_vec_init(0, 0, 0);
	col = ft_vec_add(col, ft_color_from_lights(data, ray, intersect, 0));
	if (g_log & B_COLORING)
	{
		printf("[COLORING - LAMBERTIAN] col after direct illumination : ");
		ft_vec_display(col, 1);
	}
	col = ft_vec_add(col, ft_color_from_global_illum(data, intersect));
	if (g_log & B_COLORING)
	{
		printf("[COLORING - LAMBERTIAN] col after global illumination : ");
		ft_vec_display(col, 1);
	}
	col = ft_vec_add(col, ft_color_from_ambient(data));
	if (g_log & B_COLORING)
	{
		printf("[COLORING - LAMBERTIAN] col after ambient : ");
		ft_vec_display(col, 1);
	}
	return (ft_vec_scale_by_vec(col, intersect->albedo));
}

// Gives the contribution, at a point, from sources in the direction
// exactly symmetric by the normal to the ray direction
// 		by computing the symmetric ray and sending it into the scene
t_vec	ft_color_from_reflection(t_data *data, t_ray ray,
	t_intersection *intersect)
{
	t_vec	col;
	t_vec	reflection_vec;
	t_ray	reflection_ray;

	reflection_vec = ft_vec_symmetric_by_normal(ray.dir, intersect->normal);
	reflection_vec = ft_vec_setnorm(reflection_vec, 1);
	reflection_ray = ft_ray_init_by_dir(intersect->crosspoint, reflection_vec);
	data->recurr_lvl++;
	col = ft_draw_one_ray(data, reflection_ray);
	data->recurr_lvl--;
	return (col);
}

// Determines color at a point of a metal object
// A metal (mirror) object reflects the light it receives in a direction
// exactly symmetric by the normal to the incoming directions
// So we only need to sum light received from the sym direction to the ray
// 		- light received from lights -> using the specular of Phong
// 			(this is a makeshift temporary solution)
// 		- light reveived from other scene objects
// 			-> sending a new "reflection" ray in the symmetric direction
t_vec	ft_color_ray_at_object_metal(t_data *data, t_ray ray,
	t_intersection *intersect)
{
	t_vec	col;

	if (g_log & B_COLORING)
	{
		printf("[COLORING - METAL] begin, normal vector at crosspoint : ");
		ft_vec_display(intersect->normal, 1);
	}
	col = ft_vec_init(0, 0, 0);
	col = ft_vec_add(col, ft_color_from_lights(data, ray, intersect, 1));
	if (g_log & B_COLORING)
	{
		printf("[COLORING - LAMBERTIAN] col after specular : ");
		ft_vec_display(col, 1);
	}
	col = ft_vec_add(col, ft_color_from_reflection(data, ray, intersect));
	if (g_log & B_COLORING)
	{
		printf("[COLORING - LAMBERTIAN] col after reflection : ");
		ft_vec_display(col, 1);
	}
	return (ft_vec_scale_by_vec(col, intersect->albedo));
}

// Calculates the color of the intersection point between ray and object
// by calling the adequate function according to the material intersected
// <intersect.object> at NULL means the ray intersects no object,
// 		in that case the returned color is simply black
// /!\ The dot product checks if the normal vector
// 	   and the ray direction point in the same way,
// 			which means the ray hits the object's inside
t_vec	ft_color_ray_at(t_data *data, t_ray ray, t_intersection *intersect)
{
	if (!(intersect->object))
		return (ft_vec_init(0, 0, 0));
	if (ft_vec_sproduct(ray.dir, intersect->normal) > 0)
		intersect->normal = ft_vec_neg(intersect->normal);
	if (intersect->object->material == LAMBERTIAN)
		return (ft_color_ray_at_object_lambertian(data, ray, intersect));
	if (intersect->object->material == METAL)
		return (ft_color_ray_at_object_metal(data, ray, intersect));
	else
		return (ft_vec_init(0.5, 0.5, 0.5));
}
