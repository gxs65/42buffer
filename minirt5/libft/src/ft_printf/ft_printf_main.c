/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:27:32 by abedin            #+#    #+#             */
/*   Updated: 2025/01/21 01:50:59 by abedin           ###   ########.fr       */
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
	an invalid specifier implies UB
	-> in this program, the whole specifying block after '%' is just ignored
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
int	printf_varg(t_conveyer *cv, va_list ap)
{
	if (cv->spec->conv_id == PERCENT)
		return (printf_percent(cv, ap));
	else if (cv->spec->conv_id == CHAR)
		return (printf_char(cv, ap));
	else if (cv->spec->conv_id == STRING || cv->spec->conv_id == POINTER)
		return (printf_string(cv, ap));
	else if (cv->spec->conv_id == INTI || cv->spec->conv_id == INTD
		|| cv->spec->conv_id == UINT || cv->spec->conv_id == XUINT
		|| cv->spec->conv_id == XMAJUINT)
		return (printf_number(cv, ap));
	return (0);
}

// Determines what to do with the given specifier character <c>,
// 		and exits the specifying state if necessary
// Potential values of <res_spec> (cf <store_specifier>) :
//		- 0 : the specifier is valid, we can continue scanning the input string
//		- 1 : the specifier is the conversion character, we can print the arg
//		- (-1) : the specifier is invalid
int	manage_spec(t_conveyer *cv, char c, int *specifying, va_list ap)
{
	int	res_spec;
	int	count;

	count = 0;
	res_spec = store_specifier(cv->spec, c);
	if (res_spec == -1)
		*specifying = 0;
	else if (res_spec == 1)
	{
		*specifying = 0;
		count += printf_varg(cv, ap);
		init_spec(cv->spec);
	}
	return (count);
}

// Verifies if the given character is '%' :
// 		enters the specifying state if yes, prints it in <dest> otherwise
int	manage_notspec(char *dest, char c, int *specifying)
{
	if (c == '%')
	{
		*specifying = 1;
		return (0);
	}
	else
	{
		dest[0] = c;
		return (1);
	}
}

// Scans the input string and prints its characters
// one by one in the malloc'd <dest> string,
//		except when entering the "specifying" state after a '%' character :
// In this state it sends the chars to <store_specifier> to add them to <spec>,
//		- until the conversion char is found (-> print the arg using va_arg)
//		- or a char is an invalid specifier (-> UB, so here it just continues)
// <dest> is provided with an offset of <count> to the <manage> functions
// <=> as a pointer to the first character to write, not to the full string
int	ft_printf_store(char *dest, char *s, va_list ap)
{
	int			specifying;
	int			count;
	t_spec		spec;
	t_conveyer	cv;

	init_spec(&spec);
	specifying = 0;
	count = 0;
	cv.spec = &spec;
	while (*s)
	{
		cv.dest = dest + count;
		if (specifying)
			count += manage_spec(&cv, *s, &specifying, ap);
		else
			count += manage_notspec(dest + count, *s, &specifying);
		s++;
	}
	return (count);
}

// Calls <ft_printf_count> to get the size needed for <result> string,
// allocates that string, reinitializes variadic arguments,
// calls <va_printf_store> to write the content into <result>,
// then finally writes <result> to file <fd>
// 		(only 1 call to AS-safe <write>,
// 		 so this <ft_printf> should also be AS-safe)
int	ft_printf(int fd, char *s, ...)
{
	int		size;
	char	*result;
	va_list	ap;

	if (!s)
		return (0);
	va_start(ap, s);
	size = ft_printf_count(s, ap);
	va_end(ap);
	result = malloc((size + 1) * sizeof(char));
	if (!result)
		return (0);
	ft_memset_asprt(result, size + 1, 0);
	va_start(ap, s);
	ft_printf_store(result, s, ap);
	va_end(ap);
	if (fd > 0)
		write(fd, result, size);
	free(result);
	return (size);
}
