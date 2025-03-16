/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:35:28 by alex              #+#    #+#             */
/*   Updated: 2025/03/15 12:36:23 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ft_log_objects(t_object *current)
{
	if (current)
	{
		if (current->type == SPHERE)
		{
			printf("Object %p : SPHERE mat %d, radius %f, center + color :\n",
				current, current->material, current->dist);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->albedo, 1);
		}
		else if (current->type == PLANE)
		{
			printf("Object %p : PLANE mat %d, point + normal vector + color :\n",
				current, current->material);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->normal, 1);
			ft_vec_display(current->albedo, 1);
		}
		else if (current->type == CYLINDER)
		{
			printf("Object %p : CYLINDER mat %d, radius %f, height %f, center + axis + color :\n",
				current, current->material, current->dist, current->height);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->normal, 1);
			ft_vec_display(current->albedo, 1);
			printf("\t + 2 sides defined by center and normal vector\n");
			ft_vec_display(current->sides[0].start, 0);
			ft_vec_display(current->sides[0].normal, 1);
			ft_vec_display(current->sides[1].start, 0);
			ft_vec_display(current->sides[1].normal, 1);
		}
		/*
		else if (current->type == CONE)
		{
			printf("Object %p : CONE mat %d, angle %f, height %f, center + axis + color :\n",
				current, current->material, current->dist, current->height);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->normal, 1);
			ft_vec_display(current->albedo, 1);
			printf("\t + 1 side defined by center and normal vector\n");
			ft_vec_display(current->sides[0].start, 0);
			ft_vec_display(current->sides[0].normal, 1);
		}
		*/
		else if (current->type == LIGHT)
		{
			printf("Light at intensity %f, position + color :\n", current->dist);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->albedo, 1);
		}
		ft_log_objects(current->next);
	}
}

void	ft_log_scene(t_data *data)
{
	printf("==========\n");
	printf("--- 1. Camera angle = %f, pos + dvec :\n", data->camera.fov_angle);
	ft_vec_display(data->camera.pos, 1);
	ft_vec_display(data->camera.dvec, 1);
	printf("--- 2. Ambient lightning at intensity %f, colored :\n", data->alightning.dist);
	ft_vec_display(data->alightning.albedo, 1);
	printf("--- 3. Light sources :\n");
	ft_log_objects(data->lights);
	printf("--- 4. Objects :\n");
	ft_log_objects(data->objects);
	printf("==========\n");
}
