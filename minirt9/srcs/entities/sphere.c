/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:32:41 by abedin            #+#    #+#             */
/*   Updated: 2025/03/17 17:31:15 by abedin           ###   ########.fr       */
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

	if (g_log & B_INTERSECTION)
		printf("[INTERSECTIONS - SPHERE] Computing intersection between sphere %p (radius %f) and ray\n", sphere, sphere->dist);
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

t_vec	ft_retrieve_checkered_texture(t_object *object, double ratio_vert, double ratio_hori)
{
	(void)object;
	if (((int)(ratio_vert * 12)) % 2 == ((int)(ratio_hori * 20) % 2))
		return (ft_vec_init(1, 1, 1));
	else
		return (ft_vec_init(0, 0, 0));
}

t_vec	ft_retrieve_texture(t_object *object, double ratio_vert, double ratio_hori)
{
	t_img_ppm	*ppm;
	int			x;
	int			y;

	if (object->fd_texture == 10)
		return (ft_retrieve_checkered_texture(object, ratio_vert, ratio_hori));
	
	ppm = object->ppm_texture;
	x = (int)(ratio_hori * ppm->width);
	y = (int)(ratio_vert * ppm->height);
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURE] textures by ppm fd %d, ratios->pixels : x %f -> %d, y %f -> %d\n",
			object->fd_texture, ratio_hori, x, ratio_vert, y);
	}
	return(ppm->values[y][x]);
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
	angle_vert = acos(ft_vec_sproduct(sphere->orient_up, intersect->normal)); // angle between sphere "axis" and vector center->crosspoint
	angle_hori = acos(ft_vec_sproduct(vec_hori, sphere->orient_left) / ft_vec_norm(vec_hori)); // angle between sphere's "greenwich meridian" and vector center->crosspoint
	if (ft_vec_sproduct(sphere->orient_front, vec_hori) < 0)
		angle_hori = 2 * PI - angle_hori;
	intersect->albedo = ft_retrieve_texture(sphere, angle_vert / PI, angle_hori / (2* PI));
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURE MAPPING] scalar to point hori %f, angle hori %f -> ratio %f, angle vert %f -> ratio %f; point hori : ",
			scalar_to_point_hori, angle_hori, angle_hori / (2* PI), angle_vert, angle_vert / PI);
		ft_vec_display(point_hori, 1);
		printf("[TEXTURE MAPPING] therefore albedo of sphere at this point is : ");
		ft_vec_display(intersect->albedo, 1);
	}
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
