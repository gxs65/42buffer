/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_entities1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:14:48 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:05:10 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Functions registering a scene's mandatory elements : check
// 		- that the received lines contain the correct number of words
// 		- (for ambient lighting and camera) that no other object
// 		  of this type was already registered

// Registers the ambient lighting in the <data.alightning> variable
// which is a <t_object> structure
// 		- <.dist> : brightness
// 		- <.albedo> : color
int	ft_register_alightning(char **words, int nb_words, t_data *data)
{
	printf("Registering ambient lightning\n");
	if (data->alightning.unique_object_set)
		return (3);
	if (nb_words != 3)
		return (1);
	data->alightning.dist = ft_atof(words[1]);
	if (ft_vec_from_str(&(data->alightning.albedo), words[2]))
		return (2);
	data->alightning.albedo = ft_vec_scale(data->alightning.albedo,
			1.0 / 256.0);
	data->alightning.unique_object_set = 1;
	return (0);
}

// Registers a light (there can be several lights)
// in an <t_object> structure added to the linked list <data.lights>
// 		- <.start> : position
// 		- <.dist> : brightness
// 		- <.albedo> : color
int	ft_register_lamp(char **words, int nb_words, t_data *data)
{
	t_object	*light;

	printf("Registering light\n");
	if (nb_words != 4)
		return (1);
	light = ft_object_new();
	light->type = LIGHT;
	if (ft_vec_from_str(&(light->start), words[1]))
		return (2);
	light->dist = ft_atof(words[2]);
	if (ft_vec_from_str(&(light->albedo), words[3]))
		return (2);
	light->albedo = ft_vec_scale(light->albedo, 1.0 / 256.0);
	ft_object_push_back(&(data->lights), light);
	return (0);
}

// Registers a camera in the <data.camera> variable
// which is a dedicated <t_camera> structure
// 		- <.pos> : position
// 		- <.dvec> : camera direction
// 		- <.hvec> : camera horizontality (not required in subject)
// 		- <.fov_angle> : field of view in degree (usually 90)
int	ft_register_camera(char **words, int nb_words, t_data *data)
{
	printf("Registering camera\n");
	if (data->camera.unique_object_set)
		return (3);
	if (nb_words != 5)
		return (1);
	if (ft_vec_from_str(&(data->camera.pos), words[1]))
		return (2);
	if (ft_vec_from_str(&(data->camera.dvec), words[2]))
		return (2);
	data->camera.dvec = ft_vec_setnorm(data->camera.dvec, 1);
	if (ft_vec_from_str(&(data->camera.hvec), words[3]))
		return (2);
	data->camera.hvec = ft_vec_setnorm(data->camera.hvec, 1);
	data->camera.fov_angle = ft_atof(words[4]);
	data->camera.unique_object_set = 1;
	return (0);
}
