/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:35:28 by alex              #+#    #+#             */
/*   Updated: 2025/03/17 17:48:49 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ft_log_objects(t_object *current)
{
	if (current)
	{
		printf("(object %p, next = %p) - ", current, current->next);
		if (current->type == SPHERE)
		{
			printf("SPHERE mat %d, radius %f, center + color :\n",
				current->material, current->dist);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->albedo, 1);
			if (current->fd_texture != -1)
			{
				printf("+ Texture : fd %d, ppm at %p, orientation vectors UP and LEFT anf FRONT :\n",
					current->fd_texture, current->ppm_texture);
				ft_vec_display(current->orient_up, 1);
				ft_vec_display(current->orient_left, 1);
				ft_vec_display(current->orient_front, 1);
			}
		}
		else if (current->type == PLANE)
		{
			printf(" PLANE mat %d, point + normal vector + color :\n",
				current->material);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->normal, 1);
			ft_vec_display(current->albedo, 1);
		}
		else if (current->type == CYLINDER)
		{
			printf("CYLINDER mat %d, radius %f, height %f, center + axis + color :\n",
				current->material, current->dist, current->height);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->normal, 1);
			ft_vec_display(current->albedo, 1);
			printf("+ 2 sides defined by center and normal vector\n");
			ft_vec_display(current->sides[0].start, 0);
			ft_vec_display(current->sides[0].normal, 1);
			ft_vec_display(current->sides[1].start, 0);
			ft_vec_display(current->sides[1].normal, 1);
		}
		else if (current->type == CONE)
		{
			printf("CONE mat %d, angle %f, height %f, center + axis + color :\n",
				current->material, current->dist, current->height);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->normal, 1);
			ft_vec_display(current->albedo, 1);
			printf("+ 1 side defined by center and normal vector\n");
			ft_vec_display(current->sides[0].start, 0);
			ft_vec_display(current->sides[0].normal, 1);
		}
		else if (current->type == LIGHT)
		{
			printf("LIGHT at intensity %f, position + color :\n", current->dist);
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
	printf("--- 4. Objects beginning at %p :\n", data->objects);
	ft_log_objects(data->objects);
	printf("==========\n");
}

void	ft_log_ppm_image(t_img_ppm *img)
{
	printf("==========\n");
	printf("PPM Image at %p with width %d and height %d\n", img, img->width, img->height);
	printf("First and last pixel :\n");
	ft_vec_display(img->values[0][0], 1);
	ft_vec_display(img->values[img->height - 1][img->width - 1], 1);
	printf("==========\n");
}