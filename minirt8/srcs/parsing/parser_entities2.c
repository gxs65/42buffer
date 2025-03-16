/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_entities2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:14:48 by abedin            #+#    #+#             */
/*   Updated: 2025/03/13 19:15:15 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	ft_register_alightning(char **words, int nb_words, t_data *data)
{
	printf("Registering ambient lightning\n");
	if (data->alightning.unique_object_set)
		return (3);
	if (nb_words != 3)
		return (1);
	data->alightning.dist = ft_atof(words[1]); // = ambient lightning intensity ?
	if (ft_vec_from_str(&(data->alightning.albedo), words[2])) // = ambient lightning color
		return (2);
	data->alightning.albedo = ft_vec_scale(data->alightning.albedo, 1.0 / 256.0);
	data->alightning.unique_object_set = 1;
	return (0);
}

int	ft_register_lamp(char **words, int nb_words, t_data *data)
{
	t_object	*light;

	printf("Registering light\n");
	if (nb_words != 4)
		return (1);
	light = ft_object_new();
	light->type = LIGHT;
	if (ft_vec_from_str(&(light->start), words[1])) // = light position
		return (2);
	light->dist = ft_atof(words[2]); // = light brightness ?
	if (ft_vec_from_str(&(light->albedo), words[3])) // = light color
		return (2);
	light->albedo = ft_vec_scale(light->albedo, 1.0 / 256.0);
	ft_object_push_back(&(data->lights), light);
	return (0);
}

int	ft_register_camera(char **words, int nb_words, t_data *data)
{
	printf("Registering camera\n");
	if (data->camera.unique_object_set)
		return (3);
	if (nb_words != 5)
		return (1);
	if (ft_vec_from_str(&(data->camera.pos), words[1])) // = camera position
		return (2);
	if (ft_vec_from_str(&(data->camera.dvec), words[2])) // = camera direction vector
		return (2);
	data->camera.dvec = ft_vec_setnorm(data->camera.dvec, 1);
	if (ft_vec_from_str(&(data->camera.hvec), words[3])) // = camera horizontal vector (! not given in subject)
		return (2);
	data->camera.hvec = ft_vec_setnorm(data->camera.hvec, 1);
	data->camera.fov_angle = ft_atof(words[4]); // = camera horizontal field of view in degrees
	data->camera.unique_object_set = 1;
	return (0);
}
