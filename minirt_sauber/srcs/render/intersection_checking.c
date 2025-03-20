/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_checking.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:44:24 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 17:34:02 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Calls the appropriate intersection-computing function according to type
// Returns 0 if the given ray and object never intersect, and 1 if they do
int	ft_intersect_ray_object(t_ray ray, t_object *object,
	t_interval *hit_interval, t_intersection *intersect)
{
	if (object->type == SPHERE)
		return (ft_intersect_ray_sphere(ray, object, hit_interval, intersect));
	else if (object->type == PLANE)
		return (ft_intersect_ray_plane(ray, object, hit_interval, intersect));
	else if (object->type == CYLINDER)
		return (ft_intersect_ray_cylinder(ray, object,
				hit_interval, intersect));
	else if (object->type == CONE)
		return (ft_intersect_ray_cone(ray, object, hit_interval, intersect));
	else
		return (0);
}

// Uses the intersection information to log
// and call the function determining the intersection point's color
// (if ther was an intersection with a function,
//  this function <ft_color_ray_at> will recurr to <ft_draw_one_ray>)
t_vec	ft_draw_one_ray2(t_data *data, t_ray ray, t_intersection *intersect,
	int intersected_smth)
{
	t_vec	color_components;

	if (!intersected_smth)
		intersect->object = NULL;
	if (g_log)
	{
		if (intersected_smth)
			printf("[RAY] Ray intersected object %p at t = %f\n",
				intersect->object, intersect->t);
		else
			printf("[RAY] Ray did not intersect anything\n");
	}
	color_components = ft_color_ray_at(data, ray, intersect);
	if (g_log)
	{
		printf("[RAY] -> ray color at recurr_lvl %d : ", data->recurr_lvl);
		ft_vec_display(color_components, 1);
	}
	return (color_components);
}

// Loops through objects to determine wether one of them intersects the ray,
// then if there was an intersection determines the color it produces
// /!\ Intersection interval limits :
// 		- starts at 0.01 to avoid objects behind the camera (t < 0)
// 		  and to avoid hitting the same object twice (shadow acne)
// 		- ends at INFINITE_DOUBLE initially, then is updated to be more strict
// 		  when new intersections
// /!\ <MAX_RECURR_LVL> macro to avoid too long recursivities
t_vec	ft_draw_one_ray(t_data *data, t_ray ray)
{
	t_interval		hit_interval;
	t_object		*object_current;
	t_intersection	intersect;
	int				intersected_smth;

	if (g_log)
	{
		printf("[RAY] New ray, recurr_level = %d\n", data->recurr_lvl);
	}
	if (data->recurr_lvl > MAX_RECURR_LVL)
		return (ft_vec_init(0, 0, 0));
	hit_interval = ft_interval_init(0.01, INFINITE_DOUBLE);
	object_current = data->objects;
	intersected_smth = 0;
	while (object_current)
	{
		if (ft_intersect_ray_object(ray, object_current,
				&hit_interval, &intersect))
		{
			intersected_smth = 1;
			hit_interval.max = intersect.t;
		}
		object_current = object_current->next;
	}
	return (ft_draw_one_ray2(data, ray, &intersect, intersected_smth));
}

// Checks if a ray intersects an object in the given interval :
// loops through objects to determine wether any of them intersects the ray,
// and returns immediately if so
int	ft_check_ray_shadows(t_data *data, t_ray ray, t_interval *hit_interval)
{
	t_object		*object_current;
	t_intersection	intersect;

	object_current = data->objects;
	while (object_current)
	{
		if (ft_intersect_ray_object(ray, object_current,
				hit_interval, &intersect))
			return (1);
		object_current = object_current->next;
	}
	return (0);
}
