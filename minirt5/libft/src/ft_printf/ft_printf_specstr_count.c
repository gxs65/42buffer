/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specstr_count.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:25:56 by abedin            #+#    #+#             */
/*   Updated: 2025/01/15 23:38:13 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Conv char : 'p' or 's'
// Specifiers to implement : [-], field width, precision (for 's' only)
// Method : first creates a string <base_s> containing the chars/digits,
// 		then applies precision for 's' (which reduces len if precision < len),
// 		then adds to count the max between len and field width
int	printf_string_count(va_list ap, t_spec *spec)
{
	int		len;
	char	*base_s;

	base_s = base_s_strptr(ap, spec, &len);
	if (!base_s)
		return (0);
	if (spec->conv_id == STRING && spec->precision_defined
		&& spec->precision < len && !spec->is_null)
	{
		base_s[spec->precision] = '\0';
		len = spec->precision;
	}
	free(base_s);
	return (ft_max(spec->fw, len));
}
