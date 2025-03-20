/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logs2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:08:15 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:27:47 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Logs how a texture is applied to an object, if it has one
void	ft_log_object_texture(t_object *current)
{
	printf("+ Texture : fd %d, orientation vectors UP and LEFT and FRONT :\n",
		current->fd_texture);
	ft_vec_display(current->orient_up, 0);
	ft_vec_display(current->orient_left, 0);
	ft_vec_display(current->orient_front, 1);
	if (current->fd_texture == -2)
		printf("\ttexture name : checkered\n");
	else
		printf("\ttexture name : %s, stored at %p\n",
			current->texture_name, current->ppm_texture);
}

// Logs the sides of cone and cylinder objects
void	ft_log_object_sides(t_object *current, int nb_sides)
{
	int	ind;

	ind = 0;
	while (ind < nb_sides)
	{
		printf("+ side defined by center and normal vector :\n");
		ft_vec_display(current->sides[ind].start, 0);
		ft_vec_display(current->sides[ind].normal, 1);
		ind++;
	}
}

// Logs common attributes of scene objects
void	ft_log_object_main_paras(t_object *current, int include_normal)
{
	ft_vec_display(current->start, 1);
	if (include_normal)
		ft_vec_display(current->normal, 1);
	ft_vec_display(current->albedo, 1);
}

// Continuation of <ft_log_object>
void	ft_log_objects2(t_object	*current)
{
	if (current->type == CONE)
	{
		printf("CONE mat %d, angle %f, height %f, center + axis + color :\n",
			current->material, current->dist, current->height);
		ft_log_object_main_paras(current, 1);
		ft_log_object_sides(current, 2);
	}
	else if (current->type == LIGHT)
	{
		printf("LIGHT at intensity %f, position + color :\n", current->dist);
		ft_log_object_main_paras(current, 0);
	}
	if (current->type != LIGHT && current->fd_texture != -1)
		ft_log_object_texture(current);
	ft_log_objects(current->next);
}

// Goes through a linked list of scene objects to log each one,
// according to its type
void	ft_log_objects(t_object *current)
{
	if (!current)
		return ;
	printf("------ ");
	if (current->type == SPHERE)
	{
		printf("SPHERE mat %d, radius %f, center + color :\n",
			current->material, current->dist);
		ft_log_object_main_paras(current, 0);
	}
	else if (current->type == PLANE)
	{
		printf(" PLANE mat %d, point + normal vector + color :\n",
			current->material);
		ft_log_object_main_paras(current, 1);
	}
	else if (current->type == CYLINDER)
	{
		printf("CYLINDER mat %d, radius %f, height %f, center+axis+color :\n",
			current->material, current->dist, current->height);
		ft_log_object_main_paras(current, 1);
		ft_log_object_sides(current, 2);
	}
	ft_log_objects2(current);
}
