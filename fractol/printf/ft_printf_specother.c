/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specother.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:27:32 by abedin            #+#    #+#             */
/*   Updated: 2024/11/19 10:27:35 by abedin           ###   ########.fr       */
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
int	printf_char(va_list ap, t_spec *spec)
{
	unsigned char	c;

	c = (unsigned char)va_arg(ap, int);
	if (spec->flags[1])
	{
		ft_putchar_fd(c, 1);
		ft_putnchar_fd(' ', spec->fw - 1, 1);
	}
	else
	{
		ft_putnchar_fd(' ', spec->fw - 1, 1);
		ft_putchar_fd(c, 1);
	}
	return (ft_max(1, spec->fw));
}

// Conv char : '%'
// Specifiers to implement : all specifiers are accepted,
// 		but none has any effect
int	printf_percent(va_list ap, t_spec *spec)
{
	ft_putchar_fd('%', 1);
	(void)spec;
	(void)ap;
	return (1);
}
