/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_entities1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:13:34 by abedin            #+#    #+#             */
/*   Updated: 2025/03/17 17:45:14 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	ft_register_sphere(char **words, int nb_words, t_data *data)
{
	t_object	*sphere;

	if (nb_words < 5)
		return (1);
	sphere = ft_object_new();
	if (!sphere)
		return (2);
	sphere->type = SPHERE;
	if (ft_vec_from_str(&(sphere->start), words[1])) // = sphere center
		return (2);
	sphere->dist = ft_atof(words[2]); // = sphere radius
	if (ft_vec_from_str(&(sphere->albedo), words[3])) // = sphere color
		return (2);
	sphere->albedo = ft_vec_scale(sphere->albedo, 1.0 / 256.0);
	sphere->material = ft_atoi(words[4]);
	if (nb_words >= 8)
	{
		sphere->fd_texture = ft_atoi(words[5]);
		if (ft_vec_from_str(&(sphere->orient_up), words[6]))
			return (2);
		sphere->orient_up = ft_vec_setnorm(sphere->orient_up, 1);
		if (ft_vec_from_str(&(sphere->orient_left), words[7]))
			return (2);
		sphere->orient_left = ft_vec_setnorm(sphere->orient_left, 1);
		sphere->orient_front = ft_vec_setnorm(ft_vec_vproduct(sphere->orient_up, sphere->orient_left), 1);
		sphere->ppm_texture = data->textures[sphere->fd_texture];
	}
	else
		sphere->fd_texture = -1;
	ft_object_push_back(&(data->objects), sphere);
	printf("Registering sphere in %p\n", sphere);
	return (0);
}

int	ft_register_plane(char **words, int nb_words, t_data *data)
{
	t_object	*plane;

	if (nb_words != 5)
		return (1);
	plane = ft_object_new();
	if (!plane)
		return (2);
	plane->type = PLANE;
	if (ft_vec_from_str(&(plane->start), words[1])) // = one point on the plane
		return (2);
	if (ft_vec_from_str(&(plane->normal), words[2])) // = plane normal vector
		return (2);
	plane->normal = ft_vec_setnorm(plane->normal, 1);
	if (ft_vec_from_str(&(plane->albedo), words[3])) // = plane color
		return (2);
	plane->albedo = ft_vec_scale(plane->albedo, 1.0 / 256.0);
	plane->material = ft_atoi(words[4]);
	ft_object_push_back(&(data->objects), plane);
	printf("Registering plane in %p\n", plane);
	return (0);
}

void	ft_det_cylinder_sides(t_object *cylinder)
{
	cylinder->sides[0].normal = ft_vec_copy(cylinder->normal);
	cylinder->sides[0].start = ft_vec_add(cylinder->start,
		ft_vec_scale(cylinder->sides[0].normal, cylinder->height / 2));
	cylinder->sides[1].normal = ft_vec_neg(cylinder->normal);
	cylinder->sides[1].start = ft_vec_add(cylinder->start,
		ft_vec_scale(cylinder->sides[1].normal, cylinder->height / 2));
}

int	ft_register_cylinder(char **words, int nb_words, t_data *data)
{
	t_object	*cylinder;

	if (nb_words != 7)
		return (1);
	cylinder = ft_object_new();
	if (!cylinder)
		return (2);
	cylinder->type = CYLINDER;
	if (ft_vec_from_str(&(cylinder->start), words[1])) // = center of cylinder
		return (2);
	if (ft_vec_from_str(&(cylinder->normal), words[2])) // = axis of cylinder
		return (2);
	cylinder->normal = ft_vec_setnorm(cylinder->normal, 1);
	cylinder->dist = ft_atof(words[3]);
	cylinder->height = ft_atof(words[4]);
	if (ft_vec_from_str(&(cylinder->albedo), words[5])) // = plane color
		return (2);
	cylinder->albedo = ft_vec_scale(cylinder->albedo, 1.0 / 256.0);
	cylinder->material = ft_atoi(words[6]);
	ft_det_cylinder_sides(cylinder);
	ft_object_push_back(&(data->objects), cylinder);
	printf("Registering cylinder in %p\n", cylinder);
	return (0);
}

void	ft_det_cone_sides(t_object *cone)
{
	double	radius_at_side;

	cone->sides[0].normal = ft_vec_copy(cone->normal);
	cone->sides[0].start = ft_vec_add(cone->start,
		ft_vec_scale(cone->sides[0].normal, cone->height));
	radius_at_side = tan(ft_degrees_to_radians(cone->dist)) * cone->height;
	cone->sides[0].dist = radius_at_side;
}

int	ft_register_cone(char **words, int nb_words, t_data *data)
{
	t_object	*cone;

	if (nb_words != 7)
	return (1);
	cone = ft_object_new();
	if (!cone)
	return (2);
	cone->type = CONE;
	if (ft_vec_from_str(&(cone->start), words[1])) // = center of cone
	return (2);
	if (ft_vec_from_str(&(cone->normal), words[2])) // = axis of cone
	return (2);
	cone->normal = ft_vec_setnorm(cone->normal, 1);
	cone->dist = ft_atof(words[3]); // = cone angle (angle between axis and surface)
	cone->height = ft_atof(words[4]);
	if (ft_vec_from_str(&(cone->albedo), words[5])) // = plane color
	return (2);
	cone->albedo = ft_vec_scale(cone->albedo, 1.0 / 256.0);
	cone->material = ft_atoi(words[6]);
	ft_det_cone_sides(cone);
	ft_object_push_back(&(data->objects), cone);
	printf("Registering cone in %p\n", cone);
	return (0);
}
