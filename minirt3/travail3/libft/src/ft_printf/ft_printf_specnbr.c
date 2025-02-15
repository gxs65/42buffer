/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specnbr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:03:10 by abedin            #+#    #+#             */
/*   Updated: 2025/01/16 21:43:25 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Allocates a string for the given integer taking precision into account,
// then writes the number in that string and adds precision digits
// 		- does not write the potential '-' sign in the string,
// 			because it can only be given positive numbers
// 		- modifies <spec> to apply the override of <precision> over flag [0]
// 		- management of a value of 0 AND precision of 0 :
// 			in this case, there must be nothing in base_s
char	*base_s_int(long n, t_spec *spec, int *len)
{
	int		nb_prec_digits;
	char	*res;
	char	*res2;
	int		base;

	base = 10;
	if (spec->conv_id == XUINT || spec->conv_id == XMAJUINT)
		base = 16;
	*len = ft_putnbr_base_count(n, base);
	nb_prec_digits = ft_max(0, spec->precision - *len);
	res = malloc((*len + nb_prec_digits + 1) * sizeof(char));
	if (!res)
		return ((char *) 0);
	res2 = res + (nb_prec_digits);
	(void)ft_putnbr_base_in(n, base, (spec->conv_id == XMAJUINT), &(res2));
	res[*len + nb_prec_digits] = '\0';
	if (spec->precision_defined)
		apply_precision(spec, res, len, nb_prec_digits);
	return (res);
}

// Continuation of <ft_printf_number_render> in the case of flag [0] unset
void	printf_number_render2(t_conveyer *cv, char *base_s,
	int len, int is_neg)
{
	int		nb_extras;

	nb_extras = print_extras_count(is_neg, cv->spec);
	if (cv->spec->flags[1])
	{
		(void)print_extras(cv->dest, is_neg, cv->spec);
		ft_strcat(cv->dest + nb_extras, base_s);
		ft_strcat_nchar(cv->dest + nb_extras + len, ' ',
			cv->spec->fw - len - nb_extras);
	}
	else
	{
		ft_strcat_nchar(cv->dest, ' ', cv->spec->fw - len - nb_extras);
		(void)print_extras(cv->dest
			+ ft_max(0, cv->spec->fw - len - nb_extras), is_neg, cv->spec);
		ft_strcat(cv->dest + nb_extras
			+ ft_max(0, cv->spec->fw - len - nb_extras), base_s);
	}
}

// Counts extra characters (+/-/0x/...) to add to <base_s> with print_extra,
// then displays the number in the order given by specifiers :
// 		- [0] set : print extras, then filler 0s, then number
// 		- [0] unset, and [-] set : print extras, then number, then filler
// 		- [0] unset, and [-] unset : print extras, then filler, then number
int	printf_number_render(t_conveyer *cv, char *base_s, int len, int is_neg)
{
	int		nb_extras;

	nb_extras = print_extras_count(is_neg, cv->spec);
	if (cv->spec->flags[0])
	{
		(void)print_extras(cv->dest, is_neg, cv->spec);
		ft_strcat_nchar(cv->dest + nb_extras, '0',
			cv->spec->fw - len - nb_extras);
		ft_strcat(cv->dest + nb_extras
			+ ft_max(0, cv->spec->fw - len - nb_extras), base_s);
	}
	else
		printf_number_render2(cv, base_s, len, is_neg);
	return (nb_extras);
}

// Calls <va_arg> with the type given in <spec>
long	retrieve_number(va_list ap, t_spec *spec)
{
	long	n;

	n = 0;
	if (spec->conv_id == INTI || spec->conv_id == INTD)
		n = (long)va_arg(ap, int);
	else if (spec->conv_id == UINT || spec->conv_id == XUINT
		|| spec->conv_id == XMAJUINT)
		n = (long)va_arg(ap, t_uint);
	return (n);
}

// Retrieves the number to print,
// receives a malloc's string containing it from <base_s_int>
// on which precision HAS ALREADY been applied,
// then finally prints that string into string <dest>
// while respecting order given by specifiers (function <printf_number_render>)
// Specifiers to implement :	fw, precision, [-], [0]		for all
// 								[+], [ ]					for signed nbs
// 								[#]							for unsigned nbs
// /!\ conversion specifiers %i and %d are functionally equivalent for output
// /!\ same function <base_s_int> called for signed and unsigned nbs, because :
// 		- the number (stored in <n>) is transmitted as a positive number,
// 			with boolean <is_neg> to keep track of wether it was negative
// 		- the difference is specifier to handle is managed by <print_extras>,
// 			called later in <printf_number_render>
int	printf_number(t_conveyer *cv, va_list ap)
{
	long	n;
	int		len;
	int		is_neg;
	int		nb_extras;
	char	*base_s;

	n = retrieve_number(ap, cv->spec);
	is_neg = 0;
	if (n < 0)
	{
		is_neg = 1;
		n = n * -1;
	}
	if (n == 0)
		cv->spec->is_null = 1;
	base_s = base_s_int(n, cv->spec, &len);
	if (!base_s)
		return (0);
	apply_overrides(cv->spec);
	nb_extras = printf_number_render(cv, base_s, len, is_neg);
	free(base_s);
	return (ft_max(cv->spec->fw, len + nb_extras));
}
