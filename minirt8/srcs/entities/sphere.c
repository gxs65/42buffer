/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:32:41 by abedin            #+#    #+#             */
/*   Updated: 2025/03/15 12:35:15 by administyra      ###   ########.fr       */
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

t_vec	ft_retrieve_texture(int fd_texture, double angle_vert, double angle_hori)
{
	double	vert;
	double	hori;

	(void)fd_texture;
	vert = angle_vert / PI;
	hori = angle_hori / (2 * PI);
	if (g_log)
	{
		printf("ft_retrieve_texture received angle vert %f -> %f\n", angle_vert, vert);
		printf("ft_retrieve_texture received angle hori %f -> %f\n", angle_hori, hori);
	}
	if (((int)(vert * 10)) / 2 == 0)
		return (ft_vec_init(255, 255, 255));
	else
		return (ft_vec_init(0, 0, 0));
}

void	ft_sphere_apply_texture(t_ray ray, t_object *sphere, t_intersection *intersect)
{
	double	scalar_to_point_hori;
	double	angle_hori;
	double	angle_vert;
	t_vec	point_hori;
	t_vec	vec_hori;

	(void)ray;
	if (sphere->fd_texture == -1)
		return ;
	scalar_to_point_hori = ft_vec_sproduct(sphere->start, sphere->orient_up)
						 - ft_vec_sproduct(intersect->crosspoint, sphere->orient_up);
	point_hori = ft_vec_add(intersect->crosspoint, ft_vec_scale(sphere->orient_up, scalar_to_point_hori));
	vec_hori = ft_vec_substr(point_hori, sphere->start);
	angle_vert = acos(ft_vec_sproduct(sphere->orient_up, intersect->normal));
	angle_hori = acos(ft_vec_sproduct(vec_hori, sphere->orient_left) / ft_vec_norm(vec_hori));
	if (g_log)
	{
		printf("scalar to point hori %f, angle hori %f, angle vert %f ; point hori :\n",
			scalar_to_point_hori, angle_hori, angle_vert);
		ft_vec_display(point_hori, 1);
	}
	intersect->albedo = ft_retrieve_texture(sphere->fd_texture, angle_vert, angle_hori);
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
	intersect->albedo = sphere->albedo;
	ft_sphere_apply_texture(ray, sphere, intersect);
	return (1);
}
