/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_operations.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:19:59 by abedin            #+#    #+#             */
/*   Updated: 2025/03/13 19:20:06 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec	ft_ray_at(t_ray ray, double p)
{
	t_vec	res;

	res = ft_vec_scale(ray.dir, p);
	res = ft_vec_add(res, ray.orig);
	return (res);
}

t_ray	ft_ray_init_by_point(t_vec orig, t_vec point_on_ray)
{
	t_ray	res;

	res.orig = ft_vec_copy(orig);
	res.dir = ft_vec_setnorm(ft_vec_substr(point_on_ray, orig), 1);
	return (res);
}

t_ray	ft_ray_init_by_dir(t_vec orig, t_vec dir)
{
	t_ray	res;

	res.orig = ft_vec_copy(orig);
	res.dir = ft_vec_setnorm(dir, 1);
	return (res);
}

// Displays the given ray with or without a trailing newline
void	ft_ray_display(t_ray ray, int newline)
{
	printf("start (%f,%f,%f) + t*(%f,%f,%f)",
		ray.orig.x, ray.orig.y, ray.orig.z,
		ray.dir.x, ray.dir.y, ray.dir.z);
	if (newline)
		printf("\n");
	else
		printf(" ");
}
