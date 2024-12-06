/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specnbr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:03:10 by abedin            #+#    #+#             */
/*   Updated: 2024/12/04 15:22:38 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Applies the effects of a defined precision
// 			(also a precision of 0 with only specifier '.' and no size)
// 		- if precision > length of the written number, add '0' char fillers
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

// Final choice of ordering of the elements to print :
// 		the extras '0x' and '+'/' ', the filling spaces, the filling 0s 
int	printf_number_render(t_spec *spec, char *base_s, int len, int is_neg)
{
	int		nb_extras;

	nb_extras = print_extras(is_neg, spec, -1);
	if (spec->flags[0])
	{
		(void)print_extras(is_neg, spec, 1);
		ft_putnchar_fd('0', spec->fw - len - nb_extras, 1);
		ft_putstr_fd(base_s, 1);
	}
	else
	{
		if (spec->flags[1])
		{
			(void)print_extras(is_neg, spec, 1);
			ft_putstr_fd(base_s, 1);
			ft_putnchar_fd(' ', spec->fw - len - nb_extras, 1);
		}
		else
		{
			ft_putnchar_fd(' ', spec->fw - len - nb_extras, 1);
			(void)print_extras(is_neg, spec, 1);
			ft_putstr_fd(base_s, 1);
		}
	}
	return (nb_extras);
}

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

// Conv char : 'i' 'd' (equivalent for output), 'x', 'X', 'u'
// Specifiers to implement : all
// Method : first creates a string <base_s> containing the digits,
// 		then adds the extras ('0x' and '+'/' '),
// 		then orders the elements according to flags [ ], [+], [0], [-]
int	printf_number(va_list ap, t_spec *spec)
{
	long	n;
	int		len;
	int		is_neg;
	int		nb_extras;
	char	*base_s;

	n = retrieve_number(ap, spec);
	is_neg = 0;
	if (n < 0)
	{
		is_neg = 1;
		n = n * -1;
	}
	if (n == 0)
		spec->is_null = 1;
	base_s = base_s_int(n, spec, &len);
	if (!base_s)
		return (0);
	apply_overrides(spec);
	nb_extras = printf_number_render(spec, base_s, len, is_neg);
	free(base_s);
	return (ft_max(spec->fw, len + nb_extras));
}
