/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:47:17 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 19:11:49 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Functions performing operations on vectors (3 dimensions) :
// vectors are used both taken in parameters as value, and returned as value
// Here : vector length, normalization, and random vectors

// Returns the norm of a vector (its "length" for our purpose) squared
// (ie. before the square root was applied)
double	ft_vec_norm_sq(t_vec v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

// Returns the norm of a vector (its "length" for our purpose)
double	ft_vec_norm(t_vec v)
{
	return (sqrt(ft_vec_norm_sq(v)));
}

// Scales a vector so that its norm is now equal to <new_norm>
// /!\ normalizing a vector <=> setting its norm to 1 with this function
// /!\ reminder : ||scalar * v|| == scalar * ||v||
t_vec	ft_vec_setnorm(t_vec v, double new_norm)
{
	t_vec	res;
	double	old_norm;

	old_norm = ft_vec_norm(v);
	res = ft_vec_scale(v, new_norm / old_norm);
	return (res);
}

// Initiates a vector with the 3 coordinates set to random values
// within the interval [min, max] (max excluded)
t_vec	ft_vec_init_random_within(double min, double max)
{
	t_vec	res;

	res = ft_vec_init(ft_rand_double_within(min, max),
			ft_rand_double_within(min, max),
			ft_rand_double_within(min, max));
	return (res);
}

// Generates random vectors in the unit cube (all coordinates within [-1, 1])
// until it finds one that is within the unit sphere,
// 		this vector is scales to be a unit vector and then returned
// /!\ Check that <norm_squared> is sufficiently big, to avoid 0 division error 
t_vec	ft_vec_init_random_unit(void)
{
	t_vec	res;
	double	norm_squared;

	while (1)
	{
		res = ft_vec_init_random_within(-1, 1);
		norm_squared = ft_vec_norm_sq(res);
		if (norm_squared > 1e-20 && norm_squared < 1)
			return (ft_vec_scale(res, 1 / sqrt(norm_squared)));
	}
}
