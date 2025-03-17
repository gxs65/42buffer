/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:31:29 by abedin            #+#    #+#             */
/*   Updated: 2025/03/17 14:25:41 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Performs the linear solving for the intersection between ray and cylinder
// using the names :
// 		\ cylinder : p a point on the spine, v the spine's direction vector, r the radius
// 		\ ray : s the origin point (camera), d the direction vector
// 		\ intermediaries : c is a candidate point, that could be on the cylinder or not
// -> Then the ray is expressed as `c = s + t1 * d` and the cylinder as `||c - (p + t2 * v)|| = r`
// 		where t2 is the scalar producing the spine point that is closest to the candidate point c
// -> we use the line/plane intersection logic to find the expression of t2 according to t1 :
// 		`t2 = v . c - v . p`
// -> we replace t2 inside the cylinder's definition, to obtain a quadratic equation on t1
// 		which has the a/b/c coefficients detailed here after development/factorisation
// (dot_vd and dot_vs_vp are only helper variables)
int	ft_intersect_ray_cylinder2(t_vec p, t_vec v, double r, t_vec s, t_vec d, double *solutions)
{
	double	dot_vd,	dot_vs_vp;
	double	ax, ay, az, bx, by, bz, cx, cy, cz;
	double	a, b, c;

	dot_vd = -1 * ft_vec_sproduct(d, v);
	dot_vs_vp = -1 * (ft_vec_sproduct(v, s) - ft_vec_sproduct(v, p));
	ax = (dot_vd * v.x + d.x) * (dot_vd * v.x + d.x);
	ay = (dot_vd * v.y + d.y) * (dot_vd * v.y + d.y);
	az = (dot_vd * v.z + d.z) * (dot_vd * v.z + d.z);
	a = ax + ay + az;

	bx = 2 * ((v.x * dot_vd) * (s.x - p.x + dot_vs_vp * v.x) + d.x * (dot_vs_vp * v.x + s.x - p.x));
	by = 2 * ((v.y * dot_vd) * (s.y - p.y + dot_vs_vp * v.y) + d.y * (dot_vs_vp * v.y + s.y - p.y));
	bz = 2 * ((v.z * dot_vd) * (s.z - p.z + dot_vs_vp * v.z) + d.z * (dot_vs_vp * v.z + s.z - p.z));
	b = bx + by + bz;

	cx = (s.x - p.x) * (s.x - p.x) + dot_vs_vp * v.x * (2 * (s.x - p.x) + dot_vs_vp * v.x);
	cy = (s.y - p.y) * (s.y - p.y) + dot_vs_vp * v.y * (2 * (s.y - p.y) + dot_vs_vp * v.y);
	cz = (s.z - p.z) * (s.z - p.z) + dot_vs_vp * v.z * (2 * (s.z - p.z) + dot_vs_vp * v.z);
	c = cx + cy + cz - r * r;

	if (ft_ops_solve_quadratic(a, b, c, solutions))
		return (-1);
	else
		return (1);
}

void	ft_intersect_ray_cylinder_body(t_ray ray, t_object *cylinder, t_interval *hit_interval, t_intersection *intersects)
{
	double	solutions[2];
	int		ind;
	double	axis_scalar;

	if (ft_intersect_ray_cylinder2(cylinder->start, cylinder->normal, cylinder->dist,
		ray.orig, ray.dir, solutions) == -1) // 0 intersections with the circular body of the cylinder
	{
		intersects[0].t = -1;
		intersects[1].t = -1;
	}
	else
	{
		ind = 0;
		while (ind < 2)
		{
			intersects[ind].object = cylinder;
			intersects[ind].t = solutions[ind];
			intersects[ind].crosspoint = ft_ray_at(ray, intersects[ind].t);
			intersects[ind].albedo = cylinder->albedo;
			if (ft_interval_belongs(hit_interval, intersects[ind].t))
			{
				axis_scalar = ft_vec_sproduct(cylinder->normal, intersects[ind].crosspoint)
					- ft_vec_sproduct(cylinder->normal, cylinder->start);
				if (ft_fabs(axis_scalar) < cylinder->height / 2)
				{
					intersects[ind].normal = ft_vec_substr(intersects[ind].crosspoint,
						ft_vec_add(cylinder->start, ft_vec_scale(cylinder->normal, axis_scalar)));
					intersects[ind].normal = ft_vec_setnorm(intersects[ind].normal, 1);
					if (g_log & B_INTERSECTION)
					{
						printf("\t\t[INTERSECTIONS - CYLINDER] Among solutions found : Cylinder body hit at axis scalar %f, intersection normal :\n\t\t", axis_scalar);
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

void	ft_intersect_ray_cylinder_sides(t_ray ray, t_object *cylinder, t_interval *hit_interval, t_intersection *intersects)
{
	int	ind;

	ind = 0;
	while (ind < 2)
	{
		if (ft_intersect_ray_side(ray, &(cylinder->sides[ind]), hit_interval, &(intersects[ind])))
		{
			if (ft_vec_norm(ft_vec_substr(intersects[ind].crosspoint, cylinder->sides[ind].start)) <= cylinder->dist)
			{
				if (g_log & B_INTERSECTION)
					printf("\t\t[INTERSECTIONS - CYLINDER] Among solutions found : Cylinder side index %d hit at ray scalar %f\n", ind, intersects[ind].t);
				intersects[ind].object = cylinder;
				intersects[ind].albedo = cylinder->albedo;
			}
			else
				intersects[ind].t = -1;
		}
		else
			intersects[ind].t = -1;
		ind++;
	}
}

int	ft_intersect_ray_cylinder(t_ray ray, t_object *cylinder, t_interval *hit_interval, t_intersection *intersect)
{
	t_intersection	intersects[4];
	int				ind;
	int				ind_tmin;

	if (g_log & B_INTERSECTION)
		printf("[INTERSECTIONS - CYLINDER] Computing intersection between cylinder %p and ray\n", cylinder);

	ft_intersect_ray_cylinder_body(ray, cylinder, hit_interval, &(intersects[0]));
	ft_intersect_ray_cylinder_sides(ray, cylinder, hit_interval, &(intersects[2]));
	
	ind_tmin = -1;
	ind = 0;
	while (ind < 4)
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
