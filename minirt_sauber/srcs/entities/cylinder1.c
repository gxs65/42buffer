/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:31:29 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 14:31:08 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Checks the intersections between the ray and the 2 sides of the cylinder
// (each cylinder side is a disc), which means
// 		1) calling <ft_intersect_ray_side> for intersection
// 		   between the ray and the full plane to which the side belongs
// 		2) checking if the intersection point found is not farther
// 		   than <cylinder.dist> (= radius) from the cylinder axis
// (then the intersection information must be updated so that the object hit
//  is the cylinder, and the flag for cylinder/cone's side is set to 1)
void	ft_intersect_ray_cylinder_sides(t_ray ray, t_object *cylinder,
	t_interval *hit_interval, t_intersection *intersects)
{
	int	ind;

	ind = 0;
	while (ind < 2)
	{
		if (ft_intersect_ray_side(ray, &(cylinder->sides[ind]),
				hit_interval, &(intersects[ind])))
		{
			if (ft_vec_norm(ft_vec_substr(intersects[ind].crosspoint,
						cylinder->sides[ind].start)) <= cylinder->dist)
			{
				if (g_log & B_INTERSECTION)
					printf("\t\tside solution : side %d hit at t = %f\n",
						ind, intersects[ind].t);
				intersects[ind].object = cylinder;
				intersects[ind].albedo = cylinder->albedo;
				intersects[ind].cyco_side = 1;
			}
			else
				intersects[ind].t = -1;
		}
		else
			intersects[ind].t = -1;
		ind++;
	}
}

// Maps the intersection point on the cylinder's body to :
// 		- vertically, a scalar representing at which height on
// 		  on the cylinder's axis the intersection occurred
// 			-> computed by comparing <axis_scalar> to cylinder's height
// 		- horizontally, the angle between cylinder's "greenwich plane"
// 		  and the vector that goes from axis to crosspoint
//				(<=> the intersection's normal vector)
// 			+ since this angle goes from 0 to PI radians while we want it
// 			to go from 0 to 2PI, we need to extend it according to wether
// 			it is on the same side as sphere's <orient_front>
void	ft_cylinder_apply_texture(t_ray ray, t_object *cylinder,
	t_intersection *intersect)
{
	double	axis_scalar;
	double	ratio_vert;
	double	ratio_hori;
	double	angle_hori;

	(void)ray;
	axis_scalar = ft_vec_sproduct(cylinder->normal, intersect->crosspoint)
		- ft_vec_sproduct(cylinder->normal, cylinder->start);
	ratio_vert = (axis_scalar + (cylinder->height / 2))
		/ (cylinder->height + 0.01);
	angle_hori = acos(ft_vec_sproduct(intersect->normal,
				cylinder->orient_left));
	if (ft_vec_sproduct(cylinder->orient_front, intersect->normal) < 0)
		angle_hori = 2 * PI - angle_hori;
	ratio_hori = angle_hori / (2 * PI);
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURE - MAPPING] angle_hori %f -> ratio hori %f\n",
			angle_hori, ratio_hori);
		printf("\t\t\taxis_scalar %f -> ratio vert%f\n",
			axis_scalar, ratio_vert);
	}
	ft_retrieve_texture(intersect, cylinder, ratio_vert, ratio_hori);
}

// Checks for intersection between ray and cylinder :
// there can be max 2 intersections with cylinder, which are either
// 		- intersections with the cylinder body (curved surface)
// 		- intersections with the cylinder sides (discs at both extremes)
// -> all intersections are collected, then the intersection with the smallest
//    ray scalar (<t> minimal) is selected
// The <ft_intersect_ray_cylinder_body/side> functions will have filled
// the <intersects> with the intersection informations,
// 		+ texture is only applied on intersection with body (not side)
int	ft_intersect_ray_cylinder(t_ray ray, t_object *cylinder,
	t_interval *hit_interval, t_intersection *intersect)
{
	t_intersection	intersects[4];
	int				ind_tmin;

	if (g_log & B_INTERSECTION)
		printf("[INTERSECTIONS - CYLINDER] Computing with %p\n", cylinder);
	ft_intersect_ray_cylinder_body(ray, cylinder, hit_interval,
		&(intersects[0]));
	ft_intersect_ray_cylinder_sides(ray, cylinder, hit_interval,
		&(intersects[2]));
	ind_tmin = ft_find_tmin_in_intersects(intersects, 4);
	if (ind_tmin == -1)
		return (0);
	else
	{
		*intersect = intersects[ind_tmin];
		if (cylinder->fd_texture != -1 && intersect->cyco_side == 0)
		{
			intersect->bitangent = cylinder->normal;
			intersect->tangent = ft_vec_setnorm(ft_vec_vproduct(
						intersect->normal, intersect->bitangent), 1);
			ft_cylinder_apply_texture(ray, cylinder, intersect);
		}
		return (1);
	}
}
