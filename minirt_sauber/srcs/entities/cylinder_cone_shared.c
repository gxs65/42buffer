/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_cone_shared.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 14:25:25 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 14:33:39 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Shared function to compute c coefficient in quadratic equation
// for the intersection between ray and cone/cylinder
t_vec	ft_intersect_cyco_a(double dot_vd, t_vec v, t_vec d)
{
	t_vec	a;

	a.x = (dot_vd * v.x + d.x) * (dot_vd * v.x + d.x);
	a.y = (dot_vd * v.y + d.y) * (dot_vd * v.y + d.y);
	a.z = (dot_vd * v.z + d.z) * (dot_vd * v.z + d.z);
	return (a);
}

// Shared function to compute c coefficient in quadratic equation
// for the intersection between ray and cone/cylinder
t_vec	ft_intersect_cyco_b(t_cycoeq *eq, double dot_vd, double dot_vs_vp)
{
	t_vec	b;

	b.x = 2 * ((eq->v.x * dot_vd) * (eq->s.x - eq->p.x + dot_vs_vp * eq->v.x)
			+ eq->d.x * (dot_vs_vp * eq->v.x + eq->s.x - eq->p.x));
	b.y = 2 * ((eq->v.y * dot_vd) * (eq->s.y - eq->p.y + dot_vs_vp * eq->v.y)
			+ eq->d.y * (dot_vs_vp * eq->v.y + eq->s.y - eq->p.y));
	b.z = 2 * ((eq->v.z * dot_vd) * (eq->s.z - eq->p.z + dot_vs_vp * eq->v.z)
			+ eq->d.z * (dot_vs_vp * eq->v.z + eq->s.z - eq->p.z));
	return (b);
}

// Shared function to compute c coefficient in quadratic equation
// for the intersection between ray and cone/cylinder
t_vec	ft_intersect_cyco_c(t_cycoeq *eq, double dot_vs_vp)
{
	t_vec	c;

	c.x = (eq->s.x - eq->p.x) * (eq->s.x - eq->p.x)
		+ dot_vs_vp * eq->v.x * (2 * (eq->s.x - eq->p.x) + dot_vs_vp * eq->v.x);
	c.y = (eq->s.y - eq->p.y) * (eq->s.y - eq->p.y)
		+ dot_vs_vp * eq->v.y * (2 * (eq->s.y - eq->p.y) + dot_vs_vp * eq->v.y);
	c.z = (eq->s.z - eq->p.z) * (eq->s.z - eq->p.z)
		+ dot_vs_vp * eq->v.z * (2 * (eq->s.z - eq->p.z) + dot_vs_vp * eq->v.z);
	return (c);
}

// Goes through an array of <nb_intersects> intersections
// to return the index of the valid intersection (<intersect.t> != -1)
// with the smallest ray scalar (<intersect.t>)
int	ft_find_tmin_in_intersects(t_intersection *intersects,
	int nb_intersects)
{
	int	ind;
	int	ind_tmin;

	ind_tmin = -1;
	ind = 0;
	while (ind < nb_intersects)
	{
		if (intersects[ind].t != -1
			&& (ind_tmin == -1 || intersects[ind].t < intersects[ind_tmin].t))
			ind_tmin = ind;
		ind++;
	}
	return (ind_tmin);
}
