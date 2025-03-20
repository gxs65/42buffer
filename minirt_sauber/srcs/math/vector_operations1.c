/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:47:07 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:50:08 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Functions performing operations on vectors (3 dimensions) :
// vectors are used both taken in parameters as value, and returned as value
// Here : vector initialization, addition and substraction

// Returns a copy of the given vector
t_vec	ft_vec_copy(t_vec v)
{
	t_vec	res;

	res.x = v.x;
	res.y = v.y;
	res.z = v.z;
	return (res);
}

// Returns the <t_vec> instance with members defined by params
t_vec	ft_vec_init(double x, double y, double z)
{
	t_vec	res;

	res.x = x;
	res.y = y;
	res.z = z;
	return (res);
}

// Returns the vector resulting from adding <v2> to <v1>
t_vec	ft_vec_add(t_vec v1, t_vec v2)
{
	t_vec	res;

	res.x = v1.x + v2.x;
	res.y = v1.y + v2.y;
	res.z = v1.z + v2.z;
	return (res);
}

// Returns the vector resulting from multiplying <v1> by -1
t_vec	ft_vec_neg(t_vec v)
{
	t_vec	res;

	res.x = -1 * v.x;
	res.y = -1 * v.y;
	res.z = -1 * v.z;
	return (res);
}

// Returns the vector resulting from substracting <v2> to <v1>
t_vec	ft_vec_substr(t_vec v1, t_vec v2)
{
	return (ft_vec_add(v1, ft_vec_neg(v2)));
}
