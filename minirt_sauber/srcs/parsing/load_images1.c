/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_images1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 21:13:14 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 21:36:04 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Determines the value to give to the <fd_texture> of one object :
// 		- (-1) if it has no texture
// 		- (-2) if it has a "checkered" texture
// 		- the index of the texture in <data.textures>, it it has the texture
// + loads all texture found this way
int	ft_det_one_texture(t_data *data, t_object *current, int *nb_textures)
{
	if (current->texture_name)
	{
		if (ft_strcmp(current->texture_name, "checkered") != 0)
		{
			data->textures[*nb_textures] = ft_load_ppm(current->texture_name);
			if (!data->textures)
				return (1);
			current->ppm_texture = data->textures[*nb_textures];
			current->fd_texture = *nb_textures;
			(*nb_textures)++;
		}
		else
			current->fd_texture = -2;
	}
	else
		current->fd_texture = -1;
	return (0);
}

// Counts the number of textures to load, ie. the number of objects
// with a non-NULL texture name that is not "checkered"
int	ft_count_textures(t_data *data)
{
	t_object	*current;
	int			nb_textures;

	current = data->objects;
	nb_textures = 0;
	while (current)
	{
		if (current->texture_name
			&& ft_strcmp(current->texture_name, "checkered") != 0)
			nb_textures++;
		current = current->next;
	}
	return (nb_textures);
}

// Counts the number of textures to load, allocates space for them,
// then loops through the objects to load their textures
int	ft_init_textures(t_data *data)
{
	int			nb_textures;
	t_object	*current;

	nb_textures = ft_count_textures(data);
	if (nb_textures == 0)
		return (0);
	data->textures = malloc(nb_textures * sizeof(t_img_ppm *));
	nb_textures = 0;
	current = data->objects;
	while (current)
	{
		if (ft_det_one_texture(data, current, &nb_textures))
			return (1);
		current = current->next;
	}
	printf("Texture loading successful : %d textures loaded\n", nb_textures);
	data->nb_textures = nb_textures;
	return (0);
}
