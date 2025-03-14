/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:32:27 by abedin            #+#    #+#             */
/*   Updated: 2025/03/13 19:32:27 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Performs the linear solving for the intersection between ray and plane
// using the names :
// 		\ for the ray : s is origin vector, d is direction vector
// 		\ for the plane : p is a point on the plane, n is the plane's normal vector
// if the ray is not parallel to the plane (<=> n.d == 0),
// it must have a point on the plane, given by equation : n . ((s + t d) - p) == 0
// 		which yields the unique solution : t = (n.p - n.s) / n.d
// 			(the case of a division by 0 is precisely the case of parallelism)
double	ft_intersect_ray_plane2(t_ray ray, t_side *side, t_interval *hit_interval)
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

// Transforms the plane struct into a side struct for conveniency,
// then checks for intersection between ray and plane,
// 		if so, fills <intersect> with the intersection informations, mainly the normal 
int	ft_intersect_ray_plane(t_ray ray, t_object *plane, t_interval *hit_interval, t_intersection *intersect)
{
	double	solution;
	t_side	side;
		
	if (g_log)
		printf("Computing intersection between plane %p and ray\n", plane);
	side.start = plane->start;
	side.normal = plane->normal;
	solution = ft_intersect_ray_plane2(ray, &side, hit_interval);
	if (solution == -1)
		return (0);
	intersect->crosspoint = ft_ray_at(ray, solution);
	intersect->object = plane;
	intersect->t = solution;
	intersect->normal = ft_vec_copy(plane->normal);
	return (1);
}

// Checks for intersection between ray and side (a side is functionnally a plane),
// 		if so, fills <intersect> with the intersection informations, mainly the normal 
int	ft_intersect_ray_side(t_ray ray, t_side *side, t_interval *hit_interval, t_intersection *intersect)
{
	double	solution;

	solution = ft_intersect_ray_plane2(ray, side, hit_interval);
	if (solution == -1)
		return (0);
	intersect->crosspoint = ft_ray_at(ray, solution);
	intersect->object = NULL;
	intersect->t = solution;
	intersect->normal = ft_vec_copy(side->normal);
	return (1);
}
