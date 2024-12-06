/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_others.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:14:10 by abedin            #+#    #+#             */
/*   Updated: 2024/12/06 12:44:02 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Conv char : 't'
// Specifiers to implement : none (but <fw> indicates tab size)
int	printf_inttab(va_list ap, t_spec *spec)
{
	int	*tab;
	int	ind;
	int	count;

	ft_printf(1, "[ ");
	tab = va_arg(ap, int *);
	ind = 0;
	count = 0;
	while (ind < spec->fw)
	{
		count += ft_printf(1, "%d ", tab[ind]);
		ind++;
	}
	ft_printf(1, "]");
	return (count);
}

// Conv char : 'f'
// Specifiers to implement : none
int	printf_colorARGB(va_list ap, t_spec *spec)
{
	int	color;
	int	count;

	(void)spec;
	color = va_arg(ap, int);
	count = 0;
	count += ft_printf(1, "[ A-%d ", color / 0x01000000);
	count += ft_printf(1, "R-%d ", (color % 0x01000000) / 0x010000);
	count += ft_printf(1, "G-%d ", (color % 0x010000) / 0x0100);
	count += ft_printf(1, "B-%d ]", color % 0x0100);
	return (count);
}
