/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specnbr_extras.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:11:50 by abedin            #+#    #+#             */
/*   Updated: 2025/01/16 21:06:23 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Prints "extra characters", ie characters added to string AFTER precision,
// but generally BEFORE field width filling (exception with flag [0])
// 		- for signed integers, the '-'/'+'/' ' sign,
// 		  according to flags [+] and [ ]
// 		- for unsigned integers written in hexadecimal, the '0x' prefix,
// 		  according to flag [#]
int	print_extras(char *dest, int is_neg, t_spec *spec)
{
	if ((spec->conv_id == INTI || spec->conv_id == INTD)
		&& ((is_neg) || spec->flags[2] || spec->flags[3]))
	{
		if (is_neg)
			dest[0] = '-';
		else if (spec->flags[2])
			dest[0] = '+';
		else if (spec->flags[3])
			dest[0] = ' ';
		return (1);
	}
	else if ((spec->conv_id == XUINT || spec->conv_id == XMAJUINT)
		&& (spec->flags[4]) && (!spec->is_null))
	{
		dest[0] = '0';
		if (spec->conv_id == XUINT)
			dest[1] = 'x';
		else
			dest[1] = 'X';
		return (2);
	}
	return (0);
}

// Counts how many extra characters (+/-/0x/_) will have to be added
int	print_extras_count(int is_neg, t_spec *spec)
{
	if ((spec->conv_id == INTI || spec->conv_id == INTD)
		&& ((is_neg) || spec->flags[2] || spec->flags[3]))
		return (1);
	else if ((spec->conv_id == XUINT || spec->conv_id == XMAJUINT)
		&& (spec->flags[4]) && (!spec->is_null))
		return (2);
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

// Applies the effects of a defined precision
// 			(also a precision of 0 with only specifier '.' and no size)
// 		- if precision > length of the written number, add filler chars '0'
// 		- if precision == 0 and number == 0, put NOTHING in the string
// 		- in any case, deactivate potential flag [0]
void	apply_precision(t_spec *spec, char *res, int *len, int nb_prec_digits)
{
	int	ind;

	if (spec->precision > *len)
	{
		ind = 0;
		while (ind < nb_prec_digits)
		{
			res[ind] = '0';
			ind++;
		}
		*len = spec->precision;
	}
	if (spec->precision == 0 && spec->is_null)
	{
		res[0] = '\0';
		*len = 0;
	}
	spec->flags[0] = 0;
}
