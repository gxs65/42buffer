/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specother_count.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:27:32 by abedin            #+#    #+#             */
/*   Updated: 2025/01/15 23:36:16 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Functions to print a single character, or a single percent

// Conv char : 'c'
// Specifiers to implement : flags [-], field width
int	printf_char_count(va_list ap, t_spec *spec)
{
	unsigned char	c;

	c = (unsigned char)va_arg(ap, int);
	(void)c;
	return (ft_max(1, spec->fw));
}

// Conv char : '%'
// Specifiers to implement : all specifiers are accepted,
// 		but none has any effect
int	printf_percent_count(va_list ap, t_spec *spec)
{
	(void)spec;
	(void)ap;
	return (1);
}
