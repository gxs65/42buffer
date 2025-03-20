/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_operations3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:38:30 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 19:42:31 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Computes a power on an int number
// (needed because math library function <pow> is only for doubles)
long	ft_power(int base, int exponent)
{
	if (exponent == 0)
		return (1);
	else if (base == 0 || exponent < 0)
		return (0);
	else
		return (base * ft_power(base, exponent - 1));
}

// Computes the 2 solutions of quadratic equation of form `ax^2 + bx + c = 0`
// using the usual method `s1 or s2 = (-b + or - sqrt(b^2 - 4ac)) / 2a`
int	ft_ops_solve_quadratic(double a, double b, double c, double *solutions)
{
	double	discriminant;

	discriminant = b * b - 4 * a * c;
	if (a == 0 || discriminant < 0)
	{
		return (1);
	}
	solutions[0] = (-1 * b - sqrt(discriminant)) / (2 * a);
	solutions[1] = (-1 * b + sqrt(discriminant)) / (2 * a);
	return (0);
}

// Returns the vector resulting from the multiplication <m> * <v1>
// <=> realizes a trandformation of <v1> into the alternate coordinate system
// 	   defined by matrix <m>,
// 			where <m> describes where each axis vector (1,0,0),(0,1,0),(0,0,1)
// 			ends after being transformed (1 column for each axis vector)
// So <m> is an array of 3 <t_vec> vectors,
// each providing the values in one COLUMN of the matrix
t_vec	ft_vec_transform_by_matrix(t_vec v1, t_vec *m)
{
	t_vec	res;

	res.x = v1.x * m[0].x + v1.y * m[1].x + v1.z * m[2].x;
	res.y = v1.x * m[0].y + v1.y * m[1].y + v1.z * m[2].y;
	res.z = v1.x * m[0].z + v1.y * m[1].z + v1.z * m[2].z;
	return (res);
}
