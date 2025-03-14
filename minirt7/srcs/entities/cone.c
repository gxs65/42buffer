/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 18:34:52 by abedin            #+#    #+#             */
/*   Updated: 2025/03/14 19:24:49 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Performs the linear solving for the intersection between ray and cone
// using the names :
// 		\ cone : p a point on the spine, v the spine's direction vector, r the radius
// 		\ ray : s the origin point (camera), d the direction vector
// 		\ intermediaries : c is a candidate point, that could be on the cone or not
// -> Then the ray is expressed as `c = s + t1 * d` and the cone as `||c - (p + t2 * v)|| = r`
// 		where t2 is the scalar producing the spine point that is closest to the candidate point c
// -> we use the line/plane intersection logic to find the expression of t2 according to t1 :
// 		`t2 = v . c - v . p`
// -> we replace t2 inside the cone's definition, to obtain a quadratic equation on t1
// 		which has the a/b/c coefficients detailed here after development/factorisation
// (dot_vd and dot_vs_vp are only helper variables)
int	ft_intersect_ray_cone2(t_vec p, t_vec v, double angle, t_vec s, t_vec d, double *solutions)
{
	double	dot_vd,	dot_vs_vp;
	double	cone_m, cone_n;
	double	ax, ay, az, bx, by, bz, cx, cy, cz;
	double	a, b, c;

	dot_vd = -1 * ft_vec_sproduct(d, v);
	dot_vs_vp = -1 * (ft_vec_sproduct(v, s) - ft_vec_sproduct(v, p));
	cone_m = tan(ft_degrees_to_radians(angle)) * dot_vs_vp;
	cone_n = tan(ft_degrees_to_radians(angle)) * dot_vd;
	
	ax = (dot_vd * v.x + d.x) * (dot_vd * v.x + d.x);
	ay = (dot_vd * v.y + d.y) * (dot_vd * v.y + d.y);
	az = (dot_vd * v.z + d.z) * (dot_vd * v.z + d.z);
	a = ax + ay + az - (cone_n * cone_n);

	bx = 2 * ((v.x * dot_vd) * (s.x - p.x + dot_vs_vp * v.x) + d.x * (dot_vs_vp * v.x + s.x - p.x));
	by = 2 * ((v.y * dot_vd) * (s.y - p.y + dot_vs_vp * v.y) + d.y * (dot_vs_vp * v.y + s.y - p.y));
	bz = 2 * ((v.z * dot_vd) * (s.z - p.z + dot_vs_vp * v.z) + d.z * (dot_vs_vp * v.z + s.z - p.z));
	b = bx + by + bz - 2 * cone_n * cone_m;

	cx = (s.x - p.x) * (s.x - p.x) + dot_vs_vp * v.x * (2 * (s.x - p.x) + dot_vs_vp * v.x);
	cy = (s.y - p.y) * (s.y - p.y) + dot_vs_vp * v.y * (2 * (s.y - p.y) + dot_vs_vp * v.y);
	cz = (s.z - p.z) * (s.z - p.z) + dot_vs_vp * v.z * (2 * (s.z - p.z) + dot_vs_vp * v.z);
	c = cx + cy + cz - (cone_m * cone_m);

	if (ft_ops_solve_quadratic(a, b, c, solutions))
		return (-1);
	else
		return (1);
}

void	ft_intersect_ray_cone_body(t_ray ray, t_object *cone, t_interval *hit_interval, t_intersection *intersects)
{
	double	solutions[2];
	int		ind;
	double	axis_scalar;
	t_vec	axis_point;
	t_vec	axis_normal;
	t_vec	crosspoint_tangent_orthoaxis;
	t_vec	crosspoint_tangent_paraxis;

	if (ft_intersect_ray_cone2(cone->start, cone->normal, cone->dist,
		ray.orig, ray.dir, solutions) == -1) // 0 intersections with the circular body of the cone
	{
		intersects[0].t = -1;
		intersects[1].t = -1;
	}
	else
	{
		ind = 0;
		while (ind < 2)
		{
			intersects[ind].object = cone;
			intersects[ind].t = solutions[ind];
			intersects[ind].crosspoint = ft_ray_at(ray, intersects[ind].t);
			if (ft_interval_belongs(hit_interval, intersects[ind].t))
			{
				axis_scalar = ft_vec_sproduct(cone->normal, intersects[ind].crosspoint)
					- ft_vec_sproduct(cone->normal, cone->start);
				if (axis_scalar > 0 && axis_scalar < cone->height)
				{
					axis_point = ft_vec_add(cone->start, ft_vec_scale(cone->normal, axis_scalar));
					axis_normal = ft_vec_substr(intersects[ind].crosspoint, axis_point);
					crosspoint_tangent_orthoaxis = ft_vec_vproduct(cone->normal, axis_normal);
					crosspoint_tangent_paraxis = ft_vec_substr(intersects[ind].crosspoint, cone->start);
					intersects[ind].normal = ft_vec_vproduct(crosspoint_tangent_orthoaxis, crosspoint_tangent_paraxis);
					intersects[ind].normal = ft_vec_setnorm(intersects[ind].normal, 1);
					if (g_log)
					{
						printf("\t\tAmong solutions found : cone body hit at axis scalar %f, intersection normal :\n\t\t", axis_scalar);
						ft_vec_display(intersects[ind].normal, 1);
					}
				}
				else
					intersects[ind].t = -1;
			}
			else
				intersects[ind].t = -1;
			ind++;
		}
	}
}


void	ft_intersect_ray_cone_sides(t_ray ray, t_object *cone, t_interval *hit_interval, t_intersection *intersects)
{
	if (ft_intersect_ray_side(ray, &(cone->sides[0]), hit_interval, intersects))
	{
		if (ft_vec_norm(ft_vec_substr(intersects->crosspoint, cone->sides[0].start)) <= cone->sides[0].dist)
		{
			if (g_log)
				printf("\t\tAmong solutions found : cone side hit at ray scalar %f\n",  intersects->t);
			intersects->object = cone;
		}
		else
			intersects->t = -1;
	}
	else
		intersects->t = -1;
}


int	ft_intersect_ray_cone(t_ray ray, t_object *cone, t_interval *hit_interval, t_intersection *intersect)
{
	t_intersection	intersects[3];
	int				ind;
	int				ind_tmin;

	if (g_log)
		printf("Computing intersection between cone %p and ray\n", cone);

	ft_intersect_ray_cone_body(ray, cone, hit_interval, &(intersects[0]));
	ft_intersect_ray_cone_sides(ray, cone, hit_interval, &(intersects[2]));
	
	ind_tmin = -1;
	ind = 0;
	while (ind < 2)
	{
		if (intersects[ind].t != -1
			&& (ind_tmin == -1 || intersects[ind].t < intersects[ind_tmin].t))
			ind_tmin = ind;
		ind++;
	}
	if (ind_tmin == -1)
		return (0);
	else
	{
		*intersect = intersects[ind_tmin];
		return (1);
	}
}