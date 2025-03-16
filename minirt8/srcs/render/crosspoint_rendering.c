/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crosspoint_rendering.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:40:18 by abedin            #+#    #+#             */
/*   Updated: 2025/03/15 11:15:48 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec	ft_color_ray_at_object_lambertian(t_data *data, t_ray ray, t_intersection *intersect)
{
	t_object	*current_light;
	t_interval	shadow_interval;
	t_vec		light_direction;
	double		light_distance;
	double		para_ambient;
	double		para_mirrored;
	double		para_diffused;
	double		para_shiny;
	para_ambient = 0.1;
	para_mirrored = 0.02;
	para_diffused = 0.8;
	para_shiny = 40;
	t_ray		shadow_ray;
	t_vec		mirrored_light_direction;
	double		sproduct;
	double		sproduct2;
	t_vec		color_components;
	t_vec		current_color;
	t_vec	diffusion_vec;
	t_ray	diffusion_ray;
	t_vec	random_unit_vec;
	double	a;
	double	b;

	// Check for hits inside of object
	if (g_log)
	{
		printf("\t\t\tnormal vector at intersection point\n\t\t\t");
		ft_vec_display(intersect->normal, 1);
	}
	// check if the normal vector and the ray direction point in the same way, which means the ray hits the object's inside
	if (ft_vec_sproduct(ray.dir, intersect->normal) > 0)
		intersect->normal = ft_vec_neg(intersect->normal);

	// Contribution to illumination from LIGHTS
	color_components = ft_vec_init(0, 0, 0);
	current_light = data->lights;
	while (current_light)
	{
		// diffused component
		light_direction = ft_vec_substr(current_light->start, intersect->crosspoint);
		light_distance = ft_vec_norm(light_direction);
		light_direction = ft_vec_scale(light_direction, 1.0 / light_distance);
		shadow_interval.min = 0.01;
		shadow_interval.max = light_distance;
		shadow_ray = ft_ray_init_by_dir(intersect->crosspoint, light_direction);
		sproduct = ft_vec_sproduct(intersect->normal, light_direction);
		
		// mirrored component (specular)
		mirrored_light_direction = ft_vec_symmetric_by_normal(ft_vec_neg(light_direction), intersect->normal);
		sproduct2 = ft_vec_sproduct(mirrored_light_direction, ft_vec_neg(ray.dir));
		a = pow(sproduct2, para_shiny);
		b = a * para_mirrored * current_light->dist;
		if (g_log)
		{
			printf("For computing mirrored component : light direction, mirrored light direction, negative current ray direction :\n");
			ft_vec_display(light_direction, 1);
			ft_vec_display(mirrored_light_direction, 1);
			ft_vec_display(ft_vec_neg(ray.dir), 1);
			printf("+ sproduct of negative ray and mirrored light, power, final : %f  ; %f ; %f\n", sproduct2, a, b);
		}

		// adding to current color
		if (!ft_check_ray_shadows(data, shadow_ray, &shadow_interval) && sproduct > 0)
		{
			current_color =
				ft_vec_scale_by_vec(intersect->albedo,
					ft_vec_scale(current_light->albedo,
						para_diffused * current_light->dist * sproduct / (light_distance * light_distance)));
			if (sproduct2 > 0 && intersect->object->type != PLANE) // applying specular only to non-plane
			{
				current_color = ft_vec_add(current_color,
					ft_vec_scale_by_vec(intersect->albedo,
						ft_vec_scale(current_light->albedo,
						para_mirrored * current_light->dist * pow(sproduct2, para_shiny))));
			}
			else if (g_log)
				printf("Mirrored component dropped for being opposed to ray direction\n");
			color_components = ft_vec_add(color_components, current_color);
		}
		current_light = current_light->next;
	}
	if (g_log)
	{
		printf("\t\t\tColor components after applying lights : ");
		ft_vec_display(color_components, 1);
	}

	// Contribution to illumination from REFLECTION : one random ray sampled
	// diffusion skewed towards normal (diffusion vector randomly taken on the unit sphere, then offset from crosspoint by normal)
	if (1)
	{
			random_unit_vec = ft_vec_init_random_unit();
		diffusion_vec = ft_vec_add(intersect->normal, random_unit_vec);
		if (ft_vec_is_infinitesimal(diffusion_vec)) // to avoid having normal_vec and random_unit_vector canceling one another and giving a null diffusion vector	
			diffusion_vec = ft_vec_copy(intersect->normal);
		diffusion_vec = ft_vec_setnorm(diffusion_vec, 1);
		diffusion_ray = ft_ray_init_by_dir(intersect->crosspoint, diffusion_vec);
		data->recurr_lvl++;
		color_components = ft_vec_add(color_components,
			ft_vec_scale_by_vec(ft_draw_one_ray(data, diffusion_ray), intersect->albedo));
		data->recurr_lvl--;
		if (g_log)
		{
			printf("\t\t\tColor components after applying reflection : ");
			ft_vec_display(color_components, 1);
		}
	}

	// Adding ambient lighting
	double	intensity;
	intensity = ft_vec_norm(color_components); // if each RGB component remains in [0,1], this should remain < 3
	if (1)
	//if (intensity < data->alightning.dist)
	{
		color_components = ft_vec_add(ft_vec_scale(ft_vec_scale_by_vec(intersect->albedo, data->alightning.albedo), data->alightning.dist),
			ft_vec_scale(color_components, 1 - data->alightning.dist));
	}
	else
	{
		color_components = ft_vec_add(color_components,
			ft_vec_scale(ft_vec_scale_by_vec(intersect->albedo, data->alightning.albedo), data->alightning.dist));
	}
	
	return (color_components);
}

t_vec	ft_color_ray_at_object_metal(t_data *data, t_ray ray, t_intersection *intersect)
{
	(void)data;
	(void)ray;
	(void)intersect;
	return (ft_vec_init(0, 0, 0));
}

/*
t_vec	ft_color_ray_at_object_lambertian(t_data *data, t_ray ray, t_intersection *intersect)
{
	t_vec	color_components;
	t_vec	diffusion_vec;
	t_ray	diffusion_ray;
	t_vec	random_unit_vec;

	if (g_log)
	{
		printf("\t\t\tnormal vector at intersection point\n\t\t\t");
		ft_vec_display(intersect->normal, 1);
	}
	// check if the normal vector and the ray direction point in the same way, which means the ray hits the object's inside
	if (ft_vec_sproduct(ray.dir, intersect->normal) > 0)
	{
		intersect->normal = ft_vec_neg(intersect->normal);
	}
	
	// Method 2 : diffusion skewed towards normal (diffusion vector randomly taken on the unit sphere, then offset from crosspoint by normal)
	random_unit_vec = ft_vec_init_random_unit();
	diffusion_vec = ft_vec_add(intersect->normal, random_unit_vec);
	if (ft_vec_is_infinitesimal(diffusion_vec)) // to avoid having normal_vec and random_unit_vector canceling one another and giving a null diffusion vector	
	{
		if (g_log)
			printf("\t\t\tinfinitesimal diffusion vector -> reset to normal\n");
		diffusion_vec = ft_vec_copy(intersect->normal);
	}
	diffusion_vec = ft_vec_setnorm(diffusion_vec, 1);
	if (g_log)
	{
		printf("\t\t\trandom unit vector and resulting diffusion vector :\n\t\t\t");
		ft_vec_display(random_unit_vec, 1);
		printf("\t\t\t");
		ft_vec_display(diffusion_vec, 1);
	}
	
	diffusion_ray = ft_ray_init_by_dir(intersect->crosspoint, diffusion_vec);
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, diffusion_ray), intersect->object->albedo);
	data->recurr_lvl--;
	return (color_components);
}

t_vec	ft_color_ray_at_object_metal(t_data *data, t_ray ray, t_intersection *intersect)
{
	t_vec	color_components;
	t_vec	reflection_vec;
	t_ray	reflection_ray;

	// check if the normal vector and the ray direction point in the same way, which means the ray hits the object's inside
	if (ft_vec_sproduct(ray.dir, intersect->normal) > 0)
	{
		intersect->normal = ft_vec_neg(intersect->normal);
	}

	reflection_vec = ft_vec_symmetric_by_normal(ray.dir, intersect->normal);
	reflection_vec = ft_vec_setnorm(reflection_vec, 1);

	reflection_ray = ft_ray_init_by_dir(intersect->crosspoint, reflection_vec);
	data->recurr_lvl++;
	color_components = ft_vec_scale_by_vec(ft_draw_one_ray(data, reflection_ray), intersect->object->albedo);
	data->recurr_lvl--;
	return (color_components);
}
*/

// DETERMINING WHAT TYPE OF OBJECT WAS INTERSECTED

// Calculates the color of the intersection point between ray and object
// being given in <intersect> the ref to the 1st object the ray intersects
// 		and the ray scalar <t> at which this intersection happens
// <intersect.object> at NULL means the ray intersects no object,
// 		in that case the given color is only determined
// 		by whether the ray direction points to the light
t_vec	ft_color_ray_at(t_data *data, t_ray ray, t_intersection *intersect)
{
	if (!(intersect->object))
		return ft_vec_scale(data->alightning.albedo, data->alightning.dist * 0.001);
		//return (ft_color_ray_no_intersect(data, ray));
	if (intersect->object->material == LAMBERTIAN)
		return (ft_color_ray_at_object_lambertian(data, ray, intersect));
	if (intersect->object->material == METAL)
		return (ft_color_ray_at_object_metal(data, ray, intersect));
	else
		return (ft_vec_init(0.5, 0.5, 0.5));
}