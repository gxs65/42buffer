/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:32:27 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 12:52:19 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Performs the linear solving for the intersection between ray and plane
// using the names :
// 		\ for the ray : s is origin vector, d is direction vector
// 		\ for the plane : p is a point on plane, n is plane's normal vector
// /!\ intersection occurs only if ray is not parallel to plane (<=> n.d == 0)
// -> in case of intersection, the ray must have a point on the plane,
// 		given by equation : n . ((s + t d) - p) == 0
// 		which yields the unique solution : t = (n.p - n.s) / n.d
// 			(the case of a division by 0 is precisely the case of parallelism)
double	ft_intersect_ray_plane2(t_ray ray, t_side *side,
	t_interval *hit_interval)
{
	double	np;
	double	ns;
	double	nd;
	double	solution;

	np = ft_vec_sproduct(side->normal, side->start);
	ns = ft_vec_sproduct(side->normal, ray.orig);
	nd = ft_vec_sproduct(side->normal, ray.dir);
	if (ft_fabs(nd) < 1e-20)
		return (-1);
	solution = (np - ns) / nd;
	if (ft_interval_belongs(hit_interval, solution))
		return (solution);
	else
		return (-1);
}

// Maps the intersection point on the plane's surface
// to 2 scalars <scalar_left> and <scalar_front> : they are the scalars
// that must be applied to plane's <orient_left> and <orient_up> vector
// to get the intersection point
// 		(the scalars are calculated as the projections of the intersection
// 		 point on each orientation vector, through a dot product)
// Since the plane is infinite, this gives scalars within ]-inf, +inf[,
// but <ft_retrieve_texture> requires ratios within [0, 1[,
// so we need to "tile" the plane in finite rectangles :
// 		here, scalars divided by 10 (+ call to <fabs>) means
// 		that each rectangle is 10x10 units on the plane
void	ft_plane_apply_texture(t_ray ray, t_object *plane,
	t_intersection *intersect)
{
	t_vec	center_to_point;
	double	scalar_left;
	double	scalar_front;
	double	ratio_hori;
	double	ratio_vert;

	(void)ray;
	if (plane->fd_texture == -1)
		return ;
	center_to_point = ft_vec_substr(intersect->crosspoint, plane->start);
	scalar_left = (ft_vec_sproduct(center_to_point, plane->orient_left));
	scalar_front = (ft_vec_sproduct(center_to_point, plane->orient_front));
	ratio_hori = ft_fabs((scalar_left / 10) - (int)(scalar_left / 10));
	ratio_vert = ft_fabs((scalar_front / 10) - (int)(scalar_front / 10));
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURE - MAPPING] scalar_left %f -> ratio %f\n",
			scalar_left, ratio_hori);
		printf("\t\t\tscalar_front %f -> ratio %f\n", scalar_front, ratio_vert);
	}
	ft_retrieve_texture(intersect, plane, ratio_vert, ratio_hori);
}

// Transforms the plane struct into a side struct for conveniency,
// then checks for intersection between ray and plane,
// 		if so, fills <intersect> with the intersection informations
int	ft_intersect_ray_plane(t_ray ray, t_object *plane,
	t_interval *hit_interval, t_intersection *intersect)
{
	double	solution;
	t_side	side;

	if (g_log & B_INTERSECTION)
		printf("[INTERSECTIONS - PLANE] Computing with plane %p\n", plane);
	side.start = plane->start;
	side.normal = plane->normal;
	solution = ft_intersect_ray_plane2(ray, &side, hit_interval);
	if (solution == -1)
		return (0);
	intersect->crosspoint = ft_ray_at(ray, solution);
	intersect->object = plane;
	intersect->t = solution;
	intersect->normal = ft_vec_copy(plane->normal);
	intersect->albedo = plane->albedo;
	intersect->cyco_side = 0;
	if (plane->fd_texture != -1)
	{
		intersect->tangent = ft_vec_copy(plane->orient_left);
		intersect->bitangent = ft_vec_copy(plane->orient_front);
		ft_plane_apply_texture(ray, plane, intersect);
	}
	return (1);
}

// Checks for intersection between ray and side
// (a side is functionnally a plane, but has a dedicated structure <t_side>),
// 		if so, fills <intersect> with the intersection informations
// /!\ Intersection flag <intersect.cyco_side> is set to 0 here
// 	   but will be updated to 1 in the cylinder/cone's logic
// /!\ A side (from a cylinder or cone) cannot have a texture
int	ft_intersect_ray_side(t_ray ray, t_side *side,
	t_interval *hit_interval, t_intersection *intersect)
{
	double	solution;

	solution = ft_intersect_ray_plane2(ray, side, hit_interval);
	if (solution == -1)
		return (0);
	intersect->crosspoint = ft_ray_at(ray, solution);
	intersect->object = NULL;
	intersect->t = solution;
	intersect->normal = ft_vec_copy(side->normal);
	intersect->cyco_side = 0;
	return (1);
}
