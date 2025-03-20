/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_operations2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 18:38:24 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 19:10:14 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Creates an interval, knowing its lower and upper limits
t_interval	ft_interval_init(double min, double max)
{
	t_interval	res;

	res.min = min;
	res.max = max;
	res.size = max - min;
	return (res);
}

// Checks wether a given value belong to the interval
int	ft_interval_belongs(t_interval *interval, double d)
{
	if (interval->min <= d && d <= interval->max)
		return (1);
	else
		return (0);
}

// Generates a random double in [0, 1[
double	ft_rand_double(void)
{
	long	rand_max_excluded;

	rand_max_excluded = (long)RAND_MAX + 1;
	return (rand() / (double)rand_max_excluded);
}

// Generates a random double within the given interval
double	ft_rand_double_within(double min, double max)
{
	double	n;

	n = ft_rand_double();
	return (min + (max - min) * n);
}
