/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:32:41 by abedin            #+#    #+#             */
/*   Updated: 2025/03/13 19:32:41 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Performs the quadratic solving for the intersection between a ray and a sphere,
// 		returns -1 if the given ray does not intersects the sphere,
// 		returns the value of the ray scalar of intersection point if it does
// 			(the smallest of the 2 possible values if it intersects twice)
double	ft_intersect_ray_sphere2(t_ray ray, t_object *sphere, t_interval *hit_interval)
{
	double		a;
	double		b;
	double		c;
	double		solutions[2];

	if (g_log)
		printf("Computing intersection between sphere %p (radius %f) and ray\n", sphere, sphere->dist);
	a = ft_vec_sproduct(ray.dir, ray.dir);
	b = 2 * ft_vec_sproduct(ray.dir, ft_vec_substr(ray.orig, sphere->start));
	c = ft_vec_sproduct(ray.orig, ray.orig) + ft_vec_sproduct(sphere->start, sphere->start)
		- 2 * ft_vec_sproduct(ray.orig, sphere->start) - (sphere->dist * sphere->dist);
	if (ft_ops_solve_quadratic(a, b, c, solutions))
		return (-1);
	else if (ft_interval_belongs(hit_interval, solutions[0]))
		return (solutions[0]);
	else if (ft_interval_belongs(hit_interval, solutions[1]))
		return (solutions[1]);
	else
		return (-1);
}

// Checks for intersection between ray and sphere,
// 		if so, fills <intersect> with the intersection informations, mainly the normal 
int	ft_intersect_ray_sphere(t_ray ray, t_object *sphere, t_interval *hit_interval, t_intersection *intersect)
{
	double	solution;
	
	solution = ft_intersect_ray_sphere2(ray, sphere, hit_interval);
	if (solution == -1)
		return (0);
	intersect->crosspoint = ft_ray_at(ray, solution);
	intersect->object = sphere;
	intersect->t = solution;
	intersect->normal = ft_vec_setnorm(ft_vec_substr(intersect->crosspoint, sphere->start), 1);
	return (1);
}
