/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:22:32 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 21:26:08 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Continuation of <ft_atof>
double	ft_atof_digits(char *s, int ind)
{
	double	res;
	int		ind_decimals;

	res = 0;
	while (s[ind] && ft_isdigit(s[ind]))
	{
		res = res * 10 + (s[ind] - '0');
		ind++;
	}
	if (!(s[ind]) || s[ind] != '.')
		return (res);
	ind++;
	ind_decimals = 0;
	while (s[ind + ind_decimals] && ft_isdigit(s[ind + ind_decimals]))
	{
		res = res * 10 + (s[ind + ind_decimals] - '0');
		ind_decimals++;
	}
	return (res / (double)(ft_power(10, ind_decimals)));
}

// Returns the float number contained in string <s>
double	ft_atof(char *s)
{
	int		ind;
	int		sign;

	ind = 0;
	sign = 1;
	while (s[ind] && ft_isspace(s[ind]))
		ind++;
	while (s[ind] && (s[ind] == '-' || s[ind] == '+'))
	{
		if (s[ind] == '-')
			sign = sign * -1;
		ind++;
	}
	return (sign * ft_atof_digits(s, ind));
}
