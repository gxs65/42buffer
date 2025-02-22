/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specother.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:27:32 by abedin            #+#    #+#             */
/*   Updated: 2025/01/16 21:18:00 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Functions to print a single character, or a single percent

// /!\ <printf_char> receives an int from <va_arg>,
// 		because "args with low conversion rank (bool, char)
//		passed as variadic args are promoted to int"
// 	-> int received and then (explicitly) converted to unsigned char

// Conv char : 'c'
// Specifiers to implement : flags [-], field width
int	printf_char(t_conveyer *cv, va_list ap)
{
	unsigned char	c;

	c = (unsigned char)va_arg(ap, int);
	if (cv->spec->flags[1])
	{
		cv->dest[0] = c;
		ft_strcat_nchar(cv->dest + 1, ' ', cv->spec->fw - 1);
	}
	else
	{
		ft_strcat_nchar(cv->dest, ' ', cv->spec->fw - 1);
		cv->dest[ft_max(0, cv->spec->fw - 1)] = c;
	}
	return (ft_max(1, cv->spec->fw));
}

// Conv char : '%'
// Specifiers to implement : all specifiers are accepted,
// 		but none has any effect
int	printf_percent(t_conveyer *cv, va_list ap)
{
	cv->dest[0] = '%';
	(void)(cv->spec);
	(void)ap;
	return (1);
}
