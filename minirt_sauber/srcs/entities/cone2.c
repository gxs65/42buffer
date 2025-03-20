/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 15:10:56 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 16:27:58 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Computes the solutions to the quadratic equation for the intersection
// netween ray and cone
// Equation is derived the same way as for the cylinder, except the distance
// from candidate ray to axis is compared against a variable radius
// (`radius = tan(cone_angle) * axis_scalar`, bigger when farther from cone tip)
// 		-> new cone equation `||c - (p + t2 * v)|| = tan(angle) * t2`
// 			producing additionnal terms added to coefficients a/b/c
int	ft_intersect_ray_cone2(t_cycoeq *eq, double angle, double *solutions)
{
	double	dot_vd[2];
	double	dot_vs_vp[2];
	t_vec	comp;
	t_vec	abc;

	dot_vd[0] = -1 * ft_vec_sproduct(eq->d, eq->v);
	dot_vd[1] = tan(ft_degrees_to_radians(angle)) * dot_vd[0];
	dot_vs_vp[0] = -1 * (ft_vec_sproduct(eq->v, eq->s)
			- ft_vec_sproduct(eq->v, eq->p));
	dot_vs_vp[1] = tan(ft_degrees_to_radians(angle)) * dot_vs_vp[0];
	comp = ft_intersect_cyco_a(dot_vd[0], eq->v, eq->d);
	abc.x = comp.x + comp.y + comp.z - (dot_vd[1] * dot_vd[1]);
	comp = ft_intersect_cyco_b(eq, dot_vd[0], dot_vs_vp[0]);
	abc.y = comp.x + comp.y + comp.z - (2 * dot_vs_vp[1] * dot_vd[1]);
	comp = ft_intersect_cyco_c(eq, dot_vs_vp[0]);
	abc.z = comp.x + comp.y + comp.z - (dot_vs_vp[1] * dot_vs_vp[1]);
	if (ft_ops_solve_quadratic(abc.x, abc.y, abc.z, solutions))
		return (-1);
	else
		return (1);
}

// Computes the normal at 1 point on the body of a cone :
// somewhat more complicated than the normal on the body of a cylinder
// since here surface is sloped -> the normal is also sloped,
// 		so we also compute the crosspoint's tangent and bitangent
// 		and get the normal as the vector product of these
void	ft_intersect_ray_cone_compute_normal(t_object *cone,
	t_intersection *intersect, double axis_scalar)
{
	t_vec	intermediary_normal;

	intermediary_normal = ft_vec_substr(intersect->crosspoint, ft_vec_add(
				cone->start, ft_vec_scale(cone->normal, axis_scalar)));
	intermediary_normal = ft_vec_setnorm(intermediary_normal, 1);
	intersect->tangent = ft_vec_vproduct(cone->normal, intermediary_normal);
	intersect->tangent = ft_vec_setnorm(intersect->tangent, 1);
	intersect->bitangent = ft_vec_substr(intersect->crosspoint, cone->start);
	intersect->bitangent = ft_vec_setnorm(intersect->bitangent, 1);
	intersect->normal = ft_vec_vproduct(
			intersect->tangent, intersect->bitangent);
	intersect->normal = ft_vec_setnorm(intersect->normal, 1);
}

// Performs 2 checks on the intersection solutions with cone body :
// 		1) does <t> belong to the hit interval ?
// 		2) (since the cone is not infinite) is the intersection point
// 		   close enough to the cone's center ?
// then if the checks are successful, fill <intersect>
// with the intersection information
void	ft_intersect_ray_cone_body_checks(t_ray ray, t_object *cone,
	t_interval *hit_interval, t_intersection *intersect)
{
	double	axis_scalar;

	intersect->crosspoint = ft_ray_at(ray, intersect->t);
	axis_scalar = ft_vec_sproduct(cone->normal, intersect->crosspoint)
		- ft_vec_sproduct(cone->normal, cone->start);
	if (!ft_interval_belongs(hit_interval, intersect->t)
		|| !(axis_scalar > 0 && axis_scalar < cone->height))
	{
		intersect->t = -1;
		return ;
	}
	intersect->object = cone;
	intersect->albedo = cone->albedo;
	intersect->cyco_side = 0;
	ft_intersect_ray_cone_compute_normal(cone, intersect, axis_scalar);
	if (g_log & B_INTERSECTION)
	{
		printf("\t\tbody solution : body hit at t %f, normal : ", axis_scalar);
		ft_vec_display(intersect->normal, 1);
	}
}

// Calls <ft_intersect_ray_cone2> to get solutions on <t>
// for the intersection between ray and cone body (= curved surface)
// 		- if there are no solutions, invalidates the intersections and returns
// 		- if there are, sends the solutions for the last checks
// 		  to <ft_intersect_ray_cone_body_checks>
void	ft_intersect_ray_cone_body(t_ray ray, t_object *cone,
	t_interval *hit_interval, t_intersection *intersects)
{
	double		solutions[2];
	int			ind;
	t_cycoeq	eq;

	eq.p = cone->start;
	eq.v = cone->normal;
	eq.s = ray.orig;
	eq.d = ray.dir;
	if (ft_intersect_ray_cone2(&eq, cone->dist, solutions) == -1)
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
			ft_intersect_ray_cone_body_checks(ray, cone,
				hit_interval, &(intersects[ind]));
			ind++;
		}
	}
}
