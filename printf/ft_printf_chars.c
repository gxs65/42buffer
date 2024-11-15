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
int	printf_c(va_list ap, char spec)
{
	unsigned char	c;

	c = (unsigned char)va_arg(ap, int);
	ft_putchar(c);
	(void)spec;
	return (1);
}

int	printf_s(va_list ap, char spec)
{
	char	*s;

	s = va_arg(ap, char *);
	(void)spec;
	if (!s)
		return (ft_putstr("(null)"));
	else
		return (ft_putstr(s));
}

int	printf_percent(va_list ap, char spec)
{
	ft_putchar('%');
	(void)spec;
	(void)ap;
	return (1);
}
