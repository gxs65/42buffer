/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersection_checking.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:44:24 by abedin            #+#    #+#             */
/*   Updated: 2025/03/15 11:22:35 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Returns -1 if the given ray and object never intersect,
// 		and the scalar <t> to apply to ray to get intersection point if they do
int	ft_intersect_ray_object(t_ray ray, t_object *object, t_interval *hit_interval, t_intersection *intersect)
{
	if (object->type == SPHERE)
		return (ft_intersect_ray_sphere(ray, object, hit_interval, intersect));
	else if (object->type == PLANE)
		return (ft_intersect_ray_plane(ray, object, hit_interval, intersect));
	else if (object->type == CYLINDER)
		return (ft_intersect_ray_cylinder(ray, object, hit_interval, intersect));
	else if (object->type == CONE)
		return (ft_intersect_ray_cone(ray, object, hit_interval, intersect));
	else
		return (0);
}

// Loops through objects to determine wether one of them intersects the ray,
// then if there was an intersection determines the color it produces
// 		/!\ disregards intersection with negative <t>,
// 			because it means the object is BEHIND the camera
t_vec	ft_draw_one_ray(t_data *data, t_ray ray)
{
	double			t_min;
	double			t_current;
	t_interval		hit_interval;
	t_object		*object_current;
	t_object		*object_closest;
	t_intersection	intersect;
	t_vec			color_components;

	if (g_log)
	{
		printf("Calculating for new ray, recurr_level = %d\n", data->recurr_lvl);
		ft_ray_display(ray, 1);
	}
	if (data->recurr_lvl > MAX_RECURR_LVL)
		return (ft_vec_init(0, 0, 0));
	hit_interval = ft_interval_init(0.01, INFINITE_DOUBLE);
	t_min = INFINITE_DOUBLE;
	object_current = data->objects;
	object_closest = NULL;
	while (object_current)
	{
		if (ft_intersect_ray_object(ray, object_current, &hit_interval, &intersect))
		{
			if (g_log)
				printf("\tintersection found\n");
			t_current = intersect.t;
			if (t_current != -1)
			{
				t_min = t_current;
				object_closest = object_current;
				hit_interval.max = t_min;
			}
		}
		object_current = object_current->next;
	}
	if (!object_closest)
		intersect.object = NULL;
	if (g_log)
	{
		if (object_closest)
			printf("\tRay intersected object %p at t = %f, calculating color\n", object_closest, t_min);
		else
			printf("\tRay did not intersect anything\n");
	}
	color_components = ft_color_ray_at(data, ray, &intersect);
	if (g_log)
	{
		printf("-> final ray color returned : ");
		ft_vec_display(color_components, 1);
	}
	return (color_components);
}

int	ft_check_ray_shadows(t_data *data, t_ray ray, t_interval *hit_interval)
{
	t_object		*object_current;
	t_intersection	intersect;

	if (g_log)
	{
		printf("Checking shadows on interval %f - %f for 1 ray :\n", hit_interval->min, hit_interval->max);
		ft_ray_display(ray, 1);
	}
	object_current = data->objects;
	while (object_current)
	{
		if (ft_intersect_ray_object(ray, object_current, hit_interval, &intersect))
		{
			if (g_log)
				printf("\tshadow ray did intersect object %p within interval\n", object_current);
			return (1);
		}
		object_current = object_current->next;
	}
	return (0);
}
