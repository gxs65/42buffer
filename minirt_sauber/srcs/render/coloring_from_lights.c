/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring_from_lights.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 20:41:26 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 20:41:47 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Gives the diffuse contribution from a light to a point :
// 		- higher when the angle of the light vector with the normal is small
// 			-> dot product <intersect->normal>,<light_direction>
// 		- higher when the object is close from the light
// 			-> specific factor of distance^2
t_vec	ft_color_from_light_diffuse(t_object *light, t_intersection *intersect,
	t_vec light_direction, double light_distance)
{
	double	coss;
	double	para_diffused;
	t_vec	contribution;
	double	attenuation;

	para_diffused = 0.8;
	coss = ft_vec_sproduct(intersect->normal, light_direction);
	if (coss < 0)
		return (ft_vec_init(0, 0, 0));
	else
	{
		attenuation = (para_diffused * light->dist * coss)
			/ (light_distance * light_distance);
		contribution = ft_vec_scale(light->albedo, attenuation);
		if (g_log & B_COLORING)
		{
			printf("[COLORING] diffuse term : cos = %f, contrib : ", coss);
			ft_vec_display(contribution, 1);
		}
		return (contribution);
	}
}

// Gives the specular contribution from a light to a point,
// according to the Phong model : more light when the ray direction is
// the symmetric of the the light direction
// 		-> specific factor of cos(angle between ray and true sym)^para
t_vec	ft_color_from_light_specular(t_object *light, t_intersection *intersect,
	t_ray ray, t_vec light_direction)
{
	double	coss;
	double	para_specular[2];
	t_vec	mirrored_light_direction;
	t_vec	contribution;
	double	attenuation;

	para_specular[0] = 0.02;
	para_specular[1] = 40;
	mirrored_light_direction = ft_vec_symmetric_by_normal(
			ft_vec_neg(light_direction), intersect->normal);
		coss = ft_vec_sproduct(mirrored_light_direction, ft_vec_neg(ray.dir));
	if (coss < 0 || intersect->object->type == PLANE || intersect->cyco_side)
		return (ft_vec_init(0, 0, 0));
	else
	{
		attenuation = para_specular[0] * light->dist
			* pow(coss, para_specular[1]);
		contribution = ft_vec_scale(light->albedo, attenuation);
		if (g_log & B_COLORING)
		{
			printf("[COLORING] specular term : cos = %f, contrib : ", coss);
			ft_vec_display(contribution, 1);
		}
		return (contribution);
	}
}

// Checks if the intersection "sees" the light in <light_direction>
// by building a ray towards that light, and calling <ft_ray_check_shadows>
// to compute the intersections with the scene objects
int	ft_color_light_visible(t_data *data, t_vec light_direction,
	double light_distance, t_intersection *intersect)
{
	t_ray		shadow_ray;
	t_interval	shadow_interval;

	shadow_interval.min = 0.01;
	shadow_interval.max = light_distance;
	shadow_ray = ft_ray_init_by_dir(intersect->crosspoint,
			light_direction);
	if (ft_check_ray_shadows(data, shadow_ray, &shadow_interval))
		return (0);
	else
		return (1);
}

// Gives the contribution from direct illumination at some point,
// by going throught the scene lights,
// checking if they are visible from the point,
// and then adding their specular and diffuse contributions
t_vec	ft_color_from_lights(t_data *data, t_ray ray,
	t_intersection *intersect, int only_specular)
{
	t_vec		col;
	t_object	*current_light;
	t_vec		light_direction;
	double		light_distance;

	col = ft_vec_init(0, 0, 0);
	current_light = data->lights;
	while (current_light)
	{
		light_direction = ft_vec_substr(current_light->start,
				intersect->crosspoint);
		light_distance = ft_vec_norm(light_direction);
		light_direction = ft_vec_scale(light_direction, 1.0 / light_distance);
		if (ft_color_light_visible(data, light_direction,
				light_distance, intersect))
		{
			col = ft_vec_add(col, ft_color_from_light_specular(
						current_light, intersect, ray, light_direction));
			if (!only_specular)
				col = ft_vec_add(col, ft_color_from_light_diffuse(current_light,
							intersect, light_direction, light_distance));
		}
		current_light = current_light->next;
	}
	return (col);
}

// Gives the ambient light contribution
t_vec	ft_color_from_ambient(t_data *data)
{
	t_vec	col;

	col = ft_vec_scale(data->alightning.albedo, data->alightning.dist);
	return (col);
}
