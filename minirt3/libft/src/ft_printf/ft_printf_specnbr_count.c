/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specnbr_count.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:03:10 by abedin            #+#    #+#             */
/*   Updated: 2025/01/15 22:22:02 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Retrieves the number to print,
// receives a malloc's string containing it from <base_s_int>
// on which precision HAS ALREADY been applied,
// then adds to count the max between :
// 		- the length of that string plus the extra characters
// 		- the field width
// Specifiers to implement :	fw, precision, [-], [0]		for all
// 								[+], [ ]					for signed nbs
// 								[#]							for unsigned nbs
// /!\ conversion specifiers %i and %d are functionally equivalent for output
// /!\ same function <base_s_int> called for signed and unsigned nbs, because :
// 		- the number (stored in <n>) is transmitted as a positive number,
// 			with boolean <is_neg> to keep track of wether it was negative
// 		- the difference is specifier to handle is managed by <print_extras>,
// 			called later in <printf_number_render>
int	printf_number_count(va_list ap, t_spec *spec)
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
	nb_extras = print_extras_count(is_neg, spec);
	free(base_s);
	return (ft_max(spec->fw, len + nb_extras));
}
