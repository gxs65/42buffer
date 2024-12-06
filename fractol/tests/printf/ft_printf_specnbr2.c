/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specnbr2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:11:50 by abedin            #+#    #+#             */
/*   Updated: 2024/11/19 11:12:12 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Prints "extra characters", ie characters added to string AFTER precision,
// but generally BEFORE field width filling (exception with flag [0])
// 		- for signed integers, the '-'/'+'/' ' sign,
// 		  according to flags [+] and [ ]
// 		- for unsigned integers written in hexadecimal, the '0x' prefix,
// 		  according to flag [#]
int	print_extras(int is_neg, t_spec *spec, int fd)
{
	if ((spec->conv_id == INTI || spec->conv_id == INTD)
		&& ((is_neg) || spec->flags[2] || spec->flags[3]))
	{
		if (is_neg)
			ft_putchar_fd('-', fd);
		else if (spec->flags[2])
			ft_putchar_fd('+', fd);
		else if (spec->flags[3])
			ft_putchar_fd(' ', fd);
		return (1);
	}
	else if ((spec->conv_id == XUINT || spec->conv_id == XMAJUINT)
		&& (spec->flags[4]) && (!spec->is_null))
	{
		if (spec->conv_id == XUINT)
			ft_putstr_fd("0x", fd);
		else
			ft_putstr_fd("0X", fd);
		return (2);
	}
	return (0);
}

// Applies potential overrides : [-] beats [0], [+] beats [ ]
void	apply_overrides(t_spec *spec)
{
	if (spec->flags[0] && spec->flags[1])
		spec->flags[0] = 0;
	if (spec->flags[2] && spec->flags[3])
		spec->flags[3] = 0;
}
