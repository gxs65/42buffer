/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_nbrs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:03:10 by abedin            #+#    #+#             */
/*   Updated: 2024/11/15 13:03:11 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// The void* argument is casted to long before being printed in base 16
int	printf_p(va_list ap, char spec)
{
	void	*p;

	p = va_arg(ap, void *);
	(void)spec;
	if (!p)
		return (ft_putstr("(nil)"));
	else
	{
		ft_putstr("0x");
		return (2 + ft_putnbr_base_ulong((long) p, 16, 0));
	}
}

// Conversion specifiers %i and %d are functionnally equivqlent for output,
// 	so they are implemented by the same function
int	printf_i(va_list ap, char spec)
{
	int	i;

	i = va_arg(ap, int);
	(void)spec;
	return (ft_putnbr_base((long) i, 10, 0));
}

// This function manages three specifiers (uxX),
// since they all take an unsigned int argument
int	printf_u(va_list ap, char spec)
{
	unsigned int	ui;

	ui = va_arg(ap, unsigned int);
	if (spec == 'u')
		return (ft_putnbr_base((long) ui, 10, 0));
	else
	{
		if (spec == 'x')
			return (ft_putnbr_base((long) ui, 16, 0));
		else
			return (ft_putnbr_base((long) ui, 16, 1));
	}
}
