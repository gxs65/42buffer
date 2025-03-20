/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_operations1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:21:05 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:40:37 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Minimum of 2 doubles
double	ft_fmin(double a, double b)
{
	if (a <= b)
		return (a);
	else
		return (b);
}

// Maximum of 2 doubles
double	ft_fmax(double a, double b)
{
	if (a >= b)
		return (a);
	else
		return (b);
}

// Converts an angle from degrees to radians
double	ft_degrees_to_radians(double d)
{
	return (d * PI / 180.0);
}

// Converts an angle from radians to degrees
double	ft_radians_to_degrees(double r)
{
	return (r * 180.0 / PI);
}

// Absolute value of a double
double	ft_fabs(double a)
{
	if (a < 0)
		return (a * -1);
	else
		return (a);
}
