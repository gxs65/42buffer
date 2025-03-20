/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:47:13 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 19:12:27 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Functions performing operations on vectors (3 dimensions) :
// vectors are used both taken in parameters as value, and returned as value
// Here : vector multiplication by scalar, and by other vectors

// Returns the vector resulting from multiplying <v1> by scalar <a>
t_vec	ft_vec_scale(t_vec v, double a)
{
	t_vec	res;

	res.x = a * v.x;
	res.y = a * v.y;
	res.z = a * v.z;
	return (res);
}

// Returns the vector resulting from multiplying together
// each x/y/z coordinated of <v1> and <v2>
t_vec	ft_vec_scale_by_vec(t_vec v1, t_vec v2)
{
	t_vec	res;

	res.x = v1.x * v2.x;
	res.y = v1.y * v2.y;
	res.z = v1.z * v2.z;
	return (res);
}

// Returns the scalar product of two vectors
double	ft_vec_sproduct(t_vec v1, t_vec v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// Returns the vector resulting from vector multiplications <v2> x <v1>
// (which is a vector orthogonal to both <v1> and <v2>)
t_vec	ft_vec_vproduct(t_vec v1, t_vec v2)
{
	t_vec	res;

	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	return (res);
}

// In the case of an incident vector on a surface with normal vector <normal>,
// returns the vector reflected by the surface, ie. symmetric to <incident>
// 		(as if <normal> were the axis)
// computed thanks to formula :
// `reflected = incident - 2 * (normal . incident) * normal`
t_vec	ft_vec_symmetric_by_normal(t_vec incident, t_vec normal)
{
	return (ft_vec_substr(incident,
			ft_vec_scale(normal, ft_vec_sproduct(incident, normal) * 2)));
}
