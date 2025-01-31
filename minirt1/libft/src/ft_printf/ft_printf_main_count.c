/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_main_count.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:27:32 by abedin            #+#    #+#             */
/*   Updated: 2025/01/16 21:08:01 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
Table of specifier concordance :
		fwidth	[0]		[-]		[+]		[ ]		[#]		Prec

%		y		y		y		y		y		y		y
c		y				y								
s		y				y								y
p		y				y								
d		y		y		y		y		y				y
i		y		y		y		y		y				y
u		y		y		y						y		y
x		y		y		y						y		y
X		y		y		y						y		y

1. If flags [0] or [-] are given without field width,
	they are accepted but ignored
2. Some specifiers override / deactivate other specifiers :
	- precision (when defined, ie there is a '.') deactivates flag [0]
	- flag [-] overrides flag [0]
	- flag [+] overrides flag [ ]
3a. Specifier VALIDITY is checked when reading specs (cf <store_specifier>),
	an invalid specifier implies UB -> in this program,
	the whole specifying block after '%' is just ignored
3b. Specifier CONCORDANCE is not checked :
	an unexpected spec for a given conv_id (eg [0] for '%s') implies UB
	-> in this program, the specifier is just ignored
4. According to compiler clang, multiple identical flags are not invalid,
	eg multiple [+] flags are considered as 1 [+] flag
5. Management of precision :
	- for strings, it is the MAXIMUM number of characters to print ;
		if string is NULL and precision <= 5, not even "(null)" is printed
	- for pointers, precision is not taken into account
	- for numbers, it is the MINIMUM number of digits to print ;
		if number is 0 and precision == 0, no digit is printed
*/

// Chooses the printing function to use according to the conv specifier
int	printf_varg_count(t_spec *spec, va_list ap)
{
	if (spec->conv_id == PERCENT)
		return (printf_percent_count(ap, spec));
	else if (spec->conv_id == CHAR)
		return (printf_char_count(ap, spec));
	else if (spec->conv_id == STRING || spec->conv_id == POINTER)
		return (printf_string_count(ap, spec));
	else if (spec->conv_id == INTI || spec->conv_id == INTD
		|| spec->conv_id == UINT || spec->conv_id == XUINT
		|| spec->conv_id == XMAJUINT)
		return (printf_number_count(ap, spec));
	return (0);
}

// Determines what to do with the given specifier character <c>,
// 		and exits the specifying state if necessary
int	manage_spec_count(char c, int *specifying, t_spec *spec, va_list ap)
{
	int	res_spec;
	int	count;

	count = 0;
	res_spec = store_specifier(spec, c);
	if (res_spec == -1)
		*specifying = 0;
	else if (res_spec == 1)
	{
		*specifying = 0;
		count += printf_varg_count(spec, ap);
		init_spec(spec);
	}
	return (count);
}

// Verifies if the given character is '%' :
// 		enters the specifying state if yes, adds 1 to count otherwise
int	manage_notspec_count(char c, int *specifying)
{
	if (c == '%')
	{
		*specifying = 1;
		return (0);
	}
	else
		return (1);
}

// Counts how many characters the result string will need,
// by simulating all the steps of the printf but never writing anything
int	ft_printf_count(char *s, va_list ap)
{
	int		specifying;
	int		count;
	t_spec	spec;

	init_spec(&spec);
	specifying = 0;
	count = 0;
	while (*s)
	{
		if (specifying)
			count += manage_spec_count(*s, &specifying, &spec, ap);
		else
			count += manage_notspec_count(*s, &specifying);
		s++;
	}
	return (count);
}
