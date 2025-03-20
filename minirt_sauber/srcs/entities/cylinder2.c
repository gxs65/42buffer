/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:28:42 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 15:23:50 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Performs the linear solving for the intersection between ray and cylinder
// using the parameters given inside <eq> :
// 		\ cylinder : p a point on the axis, v the spine's direction vector
// 			(+ a scalar : r the radius)
// 		\ ray : s the origin point (camera), d the direction vector
// 		\ intermediaries : c is a candidate point,
// 			the cylinder equation tells if it is on the cylinder or not
// -> the ray is expressed as `c = s + t1 * d`
// 			   and the cylinder as `||c - (p + t2 * v)|| = r`
// 		(where t2 is the axis scalar producing the axis point
// 		 that is closest to the candidate point c)
// -> we use the line/plane intersection equation
// 	  to find the expression of t2 according to t1 :
// 		`t2 = v . c - v . p`
// -> we replace t2 inside the cylinder's definition,
// 	 to obtain a quadratic equation on t1
// 		which after development/factorisation has the a/b/c coefficients
// 		detailed in the relevant functions <ft_intersect_cyco_a/b/c>
// /!\ quadratic equation for cone intersection has almost the same coeffs
// 	   with only small corrections, so the coeff a/b/c functions are shared
// /!\ dot_vd and dot_vs_vp are only helper variables
int	ft_intersect_ray_cylinder2(t_cycoeq *eq, double r, double *solutions)
{
	double	dot_vd;
	double	dot_vs_vp;
	t_vec	comp;
	t_vec	abc;

	dot_vd = -1 * ft_vec_sproduct(eq->d, eq->v);
	dot_vs_vp = -1 * (ft_vec_sproduct(eq->v, eq->s)
			- ft_vec_sproduct(eq->v, eq->p));
	comp = ft_intersect_cyco_a(dot_vd, eq->v, eq->d);
	abc.x = comp.x + comp.y + comp.z;
	comp = ft_intersect_cyco_b(eq, dot_vd, dot_vs_vp);
	abc.y = comp.x + comp.y + comp.z;
	comp = ft_intersect_cyco_c(eq, dot_vs_vp);
	abc.z = comp.x + comp.y + comp.z - r * r;
	if (ft_ops_solve_quadratic(abc.x, abc.y, abc.z, solutions))
		return (-1);
	else
		return (1);
}

// Performs 2 checks on the intersection solutions with cylinder body :
// 		1) does <t> belong to the hit interval ?
// 		2) (since the cylinder is not infinite) is the intersection point
// 		   close enough to the cylinder's center ?
// then if the checks are successful, fill <intersect>
// with the intersection information
void	ft_intersect_ray_cylinder_body_checks(t_ray ray, t_object *cylinder,
	t_interval *hit_interval, t_intersection *intersect)
{
	double	axis_scalar;

	intersect->crosspoint = ft_ray_at(ray, intersect->t);
	axis_scalar = ft_vec_sproduct(cylinder->normal, intersect->crosspoint)
		- ft_vec_sproduct(cylinder->normal, cylinder->start);
	if (!ft_interval_belongs(hit_interval, intersect->t)
		|| !(ft_fabs(axis_scalar) < cylinder->height / 2))
	{
		intersect->t = -1;
		return ;
	}
	intersect->object = cylinder;
	intersect->albedo = cylinder->albedo;
	intersect->cyco_side = 0;
	intersect->normal = ft_vec_substr(intersect->crosspoint, ft_vec_add(
				cylinder->start, ft_vec_scale(cylinder->normal, axis_scalar)));
	intersect->normal = ft_vec_setnorm(intersect->normal, 1);
	if (g_log & B_INTERSECTION)
	{
		printf("\t\tbody solution : body hit at t %f, normal : ", axis_scalar);
		ft_vec_display(intersect->normal, 1);
	}
}

// Calls <ft_intersect_ray_cylinder2> to get solutions on <t>
// for the intersection between ray and cylinder body (= curved surface)
// 		- if there are no solutions, invalidates the intersections and returns
// 		- if there are, sends the solutions for the last checks
// 		  to <ft_intersect_ray_cylinder_body_checks>
void	ft_intersect_ray_cylinder_body(t_ray ray, t_object *cylinder,
	t_interval *hit_interval, t_intersection *intersects)
{
	double		solutions[2];
	int			ind;
	t_cycoeq	eq;

	eq.p = cylinder->start;
	eq.v = cylinder->normal;
	eq.s = ray.orig;
	eq.d = ray.dir;
	if (ft_intersect_ray_cylinder2(&eq, cylinder->dist, solutions) == -1)
	{
		intersects[0].t = -1;
		intersects[1].t = -1;
	}
	else
	{
		ind = 0;
		while (ind < 2)
		{
			intersects[ind].t = solutions[ind];
			ft_intersect_ray_cylinder_body_checks(ray, cylinder,
				hit_interval, &(intersects[ind]));
			ind++;
		}
	}
}
