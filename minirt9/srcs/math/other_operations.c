/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_operations.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:21:05 by abedin            #+#    #+#             */
/*   Updated: 2025/03/13 19:29:08 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_interval	ft_interval_init(double min, double max)
{
	t_interval	res;

	res.min = min;
	res.max = max;
	res.size = max - min;
	return (res);
}

int	ft_interval_belongs(t_interval *interval, double d)
{
	if (interval->min <= d && d <=interval->max)
		return (1);
	else
		return (0);
}

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

double	ft_fmin(double a, double b)
{
	if (a <= b)
		return (a);
	else
		return (b);
}

double	ft_fmax(double a, double b)
{
	if (a >= b)
		return (a);
	else
		return (b);
}

double	ft_degrees_to_radians(double d)
{
	return (d * PI / 180.0);
}

double	ft_radians_to_degrees(double r)
{
	return (r * 180.0 / PI);
}

double	ft_rand_double(void)
{
	long	rand_max_excluded;

	rand_max_excluded = (long)RAND_MAX + 1;
	return (rand() / (double)rand_max_excluded);
}

double	ft_rand_double_within(double min, double max)
{
	double n;

	n = ft_rand_double();
	return (min + (max - min) * n);
}

double	ft_fabs(double a)
{
	if (a < 0)
		return (a * -1);
	else
		return (a);
}

long	ft_power(int base, int exponent)
{
	if (exponent == 0)
		return (1);
	else if (base == 0 || exponent < 0)
		return (0);
	else
		return (base * ft_power(base, exponent - 1));
}
