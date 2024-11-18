/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_chars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:03:19 by abedin            #+#    #+#             */
/*   Updated: 2024/11/15 13:03:21 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// /!\ Args with low conversion rank (bool, char) passed as variadic args
// 	are automatically converted to int and treated so by <va_arg>
// 	-> for %c conv identifier, an int is received from <va_arg>,
// 		and then (explicitly) converted to unsigned char
// Specifiers to implement : flags [-], field width
int	printf_char(va_list ap, t_spec *spec)
{
	unsigned char	c;

	c = (unsigned char)va_arg(ap, int);
	if (spec->flags[1])
	{
		ft_putchar(c);
		ft_putnchar(' ', spec->fw - 1);
	}
	else
	{
		ft_putnchar(' ', spec->fw - 1);
		ft_putchar(c);
	}
	return (ft_max(1, spec->fw));
}

// Specifiers to implement : all are accepeted, but none as any effect
int	printf_percent(va_list ap, t_spec spec)
{
	ft_putchar('%');
	(void)spec;
	(void)ap;
	return (1);
}

int	printf_string(va_list ap, char spec)
{
	char	*s;

	s = va_arg(ap, char *);
	(void)spec;
	if (!s)
		return (ft_putstr("(null)"));
	else
		return (ft_putstr(s));
}
