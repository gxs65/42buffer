/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_entities2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:13:34 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:04:10 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Checks if a line describing an object has additional words for a texture
// 		by comparing the number of words to the mandatory expected number
// 		of words for that object (<expected_nb_words>)
// If there are (at least) 3 additional words for the texture, registers :
// 		- <.texture_name> : the path to the image PPM file for the texture
// 		- <.orient_up>, <.orient_left> : the orientation of the texture on
// 			the object (ex. for a sphere, <.orient_up> is the sphere axis)
int	ft_register_object_texture(char **words, int nb_words,
	int expected_nb_words, t_object *object)
{
	char	**words_texture;

	if (nb_words < expected_nb_words + 3)
	{
		object->texture_name = NULL;
		return (0);
	}
	words_texture = &(words[expected_nb_words]);
	object->texture_name = ft_strdup(words_texture[0]);
	if (ft_vec_from_str(&(object->orient_up), words_texture[1]))
		return (2);
	object->orient_up = ft_vec_setnorm(object->orient_up, 1);
	if (ft_vec_from_str(&(object->orient_left), words_texture[2]))
		return (2);
	object->orient_left = ft_vec_setnorm(object->orient_left, 1);
	object->orient_front = ft_vec_setnorm(
			ft_vec_vproduct(object->orient_up, object->orient_left), 1);
	return (0);
}

// Registers a sphere's mandatory parameters (+ potentially texture) :
// 		- <.start> : position of center
// 		- <.dist> : radius
// 		- <.albedo> : color
// 		- <.material> : wether the sphere is a LAMBERTIAN or something else
int	ft_register_sphere(char **words, int nb_words, t_data *data)
{
	t_object	*sphere;

	if (nb_words < 5)
		return (1);
	sphere = ft_object_new();
	if (!sphere)
		return (2);
	sphere->type = SPHERE;
	if (ft_vec_from_str(&(sphere->start), words[1]))
		return (2);
	sphere->dist = ft_atof(words[2]);
	if (ft_vec_from_str(&(sphere->albedo), words[3]))
		return (2);
	sphere->albedo = ft_vec_scale(sphere->albedo, 1.0 / 256.0);
	sphere->material = ft_atoi(words[4]);
	if (ft_register_object_texture(words, nb_words, 5, sphere))
		return (2);
	ft_object_push_back(&(data->objects), sphere);
	printf("Registering sphere in %p\n", sphere);
	return (0);
}

// Registers a plane's mandatory parameters (+ potentially texture) :
// 		- <.start> : a point on the place
// 		- <.normal> : plane's normal vector
// 		- <.albedo> : color
// 		- <.material> : wether the plane is a LAMBERTIAN or something else
int	ft_register_plane(char **words, int nb_words, t_data *data)
{
	t_object	*plane;

	if (nb_words < 5)
		return (1);
	plane = ft_object_new();
	if (!plane)
		return (2);
	plane->type = PLANE;
	if (ft_vec_from_str(&(plane->start), words[1]))
		return (2);
	if (ft_vec_from_str(&(plane->normal), words[2]))
		return (2);
	plane->normal = ft_vec_setnorm(plane->normal, 1);
	if (ft_vec_from_str(&(plane->albedo), words[3]))
		return (2);
	plane->albedo = ft_vec_scale(plane->albedo, 1.0 / 256.0);
	plane->material = ft_atoi(words[4]);
	if (ft_register_object_texture(words, nb_words, 5, plane))
		return (2);
	ft_object_push_back(&(data->objects), plane);
	printf("Registering plane in %p\n", plane);
	return (0);
}
