/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:32:41 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 15:36:37 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Performs the quadratic solving for intersection between a ray and a sphere,
// 		returns -1 if the given ray does not intersects the sphere,
// 		returns the value of the ray scalar of intersection point if it does
// 			(the smallest of the 2 possible values if it intersects twice)
double	ft_intersect_ray_sphere2(t_ray ray, t_object *sphere,
	t_interval *hit_interval)
{
	double		a;
	double		b;
	double		c;
	double		solutions[2];

	if (g_log & B_INTERSECTION)
		printf("[INTERSECTIONS - SPHERE] Computing with %p (radius %f)\n",
			sphere, sphere->dist);
	a = ft_vec_sproduct(ray.dir, ray.dir);
	b = 2 * ft_vec_sproduct(ray.dir, ft_vec_substr(ray.orig, sphere->start));
	c = ft_vec_sproduct(ray.orig, ray.orig)
		+ ft_vec_sproduct(sphere->start, sphere->start)
		- 2 * ft_vec_sproduct(ray.orig, sphere->start)
		- (sphere->dist * sphere->dist);
	if (ft_ops_solve_quadratic(a, b, c, solutions))
		return (-1);
	else if (ft_interval_belongs(hit_interval, solutions[0]))
		return (solutions[0]);
	else if (ft_interval_belongs(hit_interval, solutions[1]))
		return (solutions[1]);
	else
		return (-1);
}

// Maps the intersection point on the sphere's surface to 2 angles
// formed between the sphere's orientation vectors/planes,
// and the vector that goes from center to crosspoint
// (<=> the intersection's normal vector) :
// 		- <angle_vert> = angle between sphere "axis" and normal vector
// 		  (= latitude)
// 		- <angle_hori> = angle between sphere "greenwich plane"
// 		  and normal (= longitude)
// 			-> harder to compute requires to first determine the projection
// 			   og the intersection point on the sphere's horizontal plane,
// 				to be able to calculate angle between (vector to) that point
// 				and sphere's <orient_left> vector
// 			+ since the angle goes from 0 to PI radians while we want it
// 			to go from 0 to 2PI, we need to extend it according to wether
// 			it is on the same side as sphere's <orient_front>
void	ft_sphere_apply_texture(t_ray ray, t_object *sphere,
	t_intersection *intersect)
{
	double	scalar_to_point_hori;
	double	angle_hori;
	double	angle_vert;
	t_vec	point_hori;
	t_vec	vec_hori;

	(void)ray;
	scalar_to_point_hori = ft_vec_sproduct(sphere->start, sphere->orient_up)
		- ft_vec_sproduct(intersect->crosspoint, sphere->orient_up);
	point_hori = ft_vec_add(intersect->crosspoint,
			ft_vec_scale(sphere->orient_up, scalar_to_point_hori));
	vec_hori = ft_vec_substr(point_hori, sphere->start);
	angle_vert = acos(ft_vec_sproduct(sphere->orient_up, intersect->normal));
	angle_hori = acos(ft_vec_sproduct(vec_hori, sphere->orient_left)
			/ ft_vec_norm(vec_hori));
	if (ft_vec_sproduct(sphere->orient_front, vec_hori) < 0)
		angle_hori = 2 * PI - angle_hori;
	ft_retrieve_texture(intersect, sphere,
		angle_vert / PI, angle_hori / (2 * PI));
	if (g_log & B_TEXTURES)
		printf("[TEXTURE - MAPPING] angle hori %f, angle vert %f\n",
			angle_hori, angle_vert);
}

// Checks for intersection between ray and sphere,
// 		if so, fills <intersect> with the intersection informations
int	ft_intersect_ray_sphere(t_ray ray, t_object *sphere,
	t_interval *hit_interval, t_intersection *intersect)
{
	double	solution;

	solution = ft_intersect_ray_sphere2(ray, sphere, hit_interval);
	if (solution == -1)
		return (0);
	intersect->crosspoint = ft_ray_at(ray, solution);
	intersect->object = sphere;
	intersect->t = solution;
	intersect->normal = ft_vec_setnorm(ft_vec_substr(
				intersect->crosspoint, sphere->start), 1);
	intersect->albedo = sphere->albedo;
	intersect->cyco_side = 0;
	if (sphere->fd_texture != -1)
	{
		intersect->tangent = ft_vec_setnorm(
				ft_vec_vproduct(sphere->orient_up, intersect->normal), 1);
		intersect->bitangent = ft_vec_setnorm(
				ft_vec_vproduct(intersect->tangent, intersect->normal), 1);
		ft_log_intersection(sphere, intersect);
		ft_sphere_apply_texture(ray, sphere, intersect);
	}
	return (1);
}
