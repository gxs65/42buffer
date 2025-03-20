/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:34:52 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 16:03:42 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Checks the intersections between the ray and the side of the cone
// (the cone side is a disc), which means
// 		1) calling <ft_intersect_ray_side> for intersection
// 		   between the ray and the full plane to which the side belongs
// 		2) checking if the intersection point found is not farther
// 		   than <cone.dist> (= radius) from the cone axis
// (then the intersection information must be updated so that the object hit
//  is the cone, and the flag for cone/cone's side is set to 1)
void	ft_intersect_ray_cone_sides(t_ray ray, t_object *cone,
	t_interval *hit_interval, t_intersection *intersects)
{
	if (ft_intersect_ray_side(ray, &(cone->sides[0]),
			hit_interval, intersects))
	{
		if (ft_vec_norm(ft_vec_substr(intersects->crosspoint,
					cone->sides[0].start)) <= cone->sides[0].dist)
		{
			if (g_log & B_INTERSECTION)
				printf("\t\tside solution : side hit at t = %f\n",
					intersects->t);
			intersects->object = cone;
			intersects->albedo = cone->albedo;
			intersects->cyco_side = 1;
		}
		else
			intersects->t = -1;
	}
	else
		intersects->t = -1;
}

// Maps the intersection point on the cone's body to :
// 		- vertically, a scalar representing at which height on
// 		  on the cone's axis the intersection occurred
// 			-> computed by comparing <axis_scalar> to cone's height
// 		- horizontally, the angle between cone's "greenwich plane"
// 		  and the vector that goes from axis to crosspoint
//				(<=> the intersection's normal vector)
// 			+ since this angle goes from 0 to PI radians while we want it
// 			to go from 0 to 2PI, we need to extend it according to wether
// 			it is on the same side as sphere's <orient_front>
void	ft_cone_apply_texture(t_ray ray, t_object *cone,
	t_intersection *intersect)
{
	double	axis_scalar;
	double	ratio_vert;
	double	ratio_hori;
	double	angle_hori;

	(void)ray;
	axis_scalar = ft_vec_sproduct(cone->normal, intersect->crosspoint)
		- ft_vec_sproduct(cone->normal, cone->start);
	ratio_vert = (axis_scalar) / (cone->height + 0.01);
	angle_hori = acos(ft_vec_sproduct(
				intersect->normal, cone->orient_left));
	if (ft_vec_sproduct(cone->orient_front, intersect->normal) < 0)
		angle_hori = 2 * PI - angle_hori;
	ratio_hori = angle_hori / (2 * PI);
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURE - MAPPING] angle_hori %f -> ratio hori %f\n",
			angle_hori, ratio_hori);
		printf("\t\t\taxis_scalar %f -> ratio vert%f\n",
			axis_scalar, ratio_vert);
	}
	ft_retrieve_texture(intersect, cone, ratio_vert, ratio_hori);
}

// Checks for intersection between ray and cone :
// there can be max 2 intersections with cone, which are either
// 		- intersections with the cone body (curved surface)
// 		- intersections with the cone side (discs at both extremes)
// -> all intersections are collected, then the intersection with the smallest
//    ray scalar (<t> minimal) is selected
// The <ft_intersect_ray_cone_body/side> functions will have filled
// the <intersects> with the intersection informations
// INCLUDING tangent and bitangent, so no need to compute them here,
// 		+ texture is only applied on intersection with body (not side)
int	ft_intersect_ray_cone(t_ray ray, t_object *cone,
	t_interval *hit_interval, t_intersection *intersect)
{
	t_intersection	intersects[3];
	int				ind_tmin;

	if (g_log & B_INTERSECTION)
		printf("[INTERSECTIONS - CONE] Computing with %p\n", cone);
	ft_intersect_ray_cone_body(ray, cone, hit_interval,
		&(intersects[0]));
	ft_intersect_ray_cone_sides(ray, cone, hit_interval,
		&(intersects[2]));
	ind_tmin = ft_find_tmin_in_intersects(intersects, 3);
	if (ind_tmin == -1)
		return (0);
	else
	{
		*intersect = intersects[ind_tmin];
		if (cone->fd_texture != -1 && intersect->cyco_side == 0)
			ft_cone_apply_texture(ray, cone, intersect);
		return (1);
	}
}
