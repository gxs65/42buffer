/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_entities3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:43:15 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:05:54 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Stores in <cylinder.sides> the two disc side of a cylinder
// 		-> their normals are the (+/-) cylinder's axis,
// 		   their centers are the cylinder's center (+/-) half its length
void	ft_det_cylinder_sides(t_object *cylinder)
{
	cylinder->sides[0].normal = ft_vec_copy(cylinder->normal);
	cylinder->sides[0].start = ft_vec_add(cylinder->start,
			ft_vec_scale(cylinder->sides[0].normal, cylinder->height / 2));
	cylinder->sides[1].normal = ft_vec_neg(cylinder->normal);
	cylinder->sides[1].start = ft_vec_add(cylinder->start,
			ft_vec_scale(cylinder->sides[1].normal, cylinder->height / 2));
}

// Registers a cylinder's mandatory parameters (+ potentially texture) :
// 		- <.start> : "center" <=> position of middle of cylinder's axis
// 		- <.normal> : "normal" <=> direction of cylinder's axis
// 		- <.dist> : radius (from axis to surface)
// 		- <.height> : "height" <=> length of axis
// 		- <.albedo> : color
// 		- <.material> : wether the cylinder is a LAMBERTIAN or something else
int	ft_register_cylinder(char **words, int nb_words, t_data *data)
{
	t_object	*cylinder;

	if (nb_words < 7)
		return (1);
	cylinder = ft_object_new();
	if (!cylinder)
		return (2);
	cylinder->type = CYLINDER;
	if (ft_vec_from_str(&(cylinder->start), words[1]))
		return (2);
	if (ft_vec_from_str(&(cylinder->normal), words[2]))
		return (2);
	cylinder->normal = ft_vec_setnorm(cylinder->normal, 1);
	cylinder->dist = ft_atof(words[3]);
	cylinder->height = ft_atof(words[4]);
	if (ft_vec_from_str(&(cylinder->albedo), words[5]))
		return (2);
	cylinder->albedo = ft_vec_scale(cylinder->albedo, 1.0 / 256.0);
	cylinder->material = ft_atoi(words[6]);
	ft_det_cylinder_sides(cylinder);
	if (ft_register_object_texture(words, nb_words, 7, cylinder))
		return (2);
	ft_object_push_back(&(data->objects), cylinder);
	printf("Registering cylinder in %p\n", cylinder);
	return (0);
}

// Stores in <cone.sides> the unique disc side of a cone
// 		-> its normal is the cone's axis, its center is opposed to cone's tip
void	ft_det_cone_sides(t_object *cone)
{
	double	radius_at_side;

	cone->sides[0].normal = ft_vec_copy(cone->normal);
	cone->sides[0].start = ft_vec_add(cone->start,
			ft_vec_scale(cone->sides[0].normal, cone->height));
	radius_at_side = tan(ft_degrees_to_radians(cone->dist)) * cone->height;
	cone->sides[0].dist = radius_at_side;
}

// Registers a cone's mandatory parameters (+ potentially texture) :
// 		- <.start> : position of cone's "tip"
// 		- <.normal> : "normal" <=> direction of cone's axis
// 		- <.dist> : angle in degrees between cone's surface and its axis
// 					(the smaller is the angle, the thinner is the cone)
// 		- <.height> : "height" <=> length of axis
// 		- <.albedo> : color
// 		- <.material> : wether the cone is a LAMBERTIAN or something else
int	ft_register_cone(char **words, int nb_words, t_data *data)
{
	t_object	*cone;

	if (nb_words < 7)
		return (1);
	cone = ft_object_new();
	if (!cone)
		return (2);
	cone->type = CONE;
	if (ft_vec_from_str(&(cone->start), words[1]))
		return (2);
	if (ft_vec_from_str(&(cone->normal), words[2]))
		return (2);
	cone->normal = ft_vec_setnorm(cone->normal, 1);
	cone->dist = ft_atof(words[3]);
	cone->height = ft_atof(words[4]);
	if (ft_vec_from_str(&(cone->albedo), words[5]))
		return (2);
	cone->albedo = ft_vec_scale(cone->albedo, 1.0 / 256.0);
	cone->material = ft_atoi(words[6]);
	ft_det_cone_sides(cone);
	if (ft_register_object_texture(words, nb_words, 7, cone))
		return (2);
	ft_object_push_back(&(data->objects), cone);
	printf("Registering cone in %p\n", cone);
	return (0);
}
