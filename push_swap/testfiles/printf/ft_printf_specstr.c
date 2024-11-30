/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_specstr.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:25:56 by abedin            #+#    #+#             */
/*   Updated: 2024/11/19 10:25:59 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Simply duplicates the given string
// Management of a NULL string pointer given to <ft_printf> :
// 		the function would receive "(null)" in <s>, and
//			- duplicate it if sufficient precision (prec > 5)
// 			- return an empty string otherwise
char	*base_s_str(char *s, int *len, t_spec *spec)
{
	char	*res;
	int		pos;

	*len = 0;
	while (s[*len] != '\0')
		(*len)++;
	res = malloc((*len + 1) * sizeof(char));
	if (!res)
		return ((char *) 0);
	pos = 0;
	while (pos < *len)
	{
		res[pos] = s[pos];
		pos++;
	}
	res[*len] = '\0';
	if (spec->is_null && spec->precision_defined
		&& spec->conv_id == STRING && spec->precision <= 5)
	{
		*len = 0;
		res[0] = '\0';
	}
	return (res);
}

// Converts the given pointer to ulong, allocates a string,
// 		then writes the number in that string in hexadecimal,
// 		then adds the '0x' prefix
// Precision on line `res2 = res + 2` : skipping the variable <res2>
// 		and directly giving `&(res + 2)` as para to <putnbr_in> does not work,
// 		because for <putnbr_in> to have a pointer (to a pointer to char)
// 		to operate with, memory for the pointer to char must still be given
// 		on the stack !
char	*base_s_ptr(void *ptr, int *len)
{
	unsigned long	n;
	char			*res;
	char			*res2;

	n = (unsigned long)ptr;
	*len = ft_putnbr_base_ulong_count(n, 16) + 2;
	res = malloc((*len + 1) * sizeof(char));
	if (!res)
		return ((char *) 0);
	res2 = res + 2;
	(void)ft_putnbr_base_ulong_in(n, 16, 0, &(res2));
	res[0] = '0';
	res[1] = 'x';
	res[*len] = '\0';
	return (res);
}

// Intermediary function to manage special cases
// of receiving a null pointer from <va_arg>
char	*base_s_strptr(va_list ap, t_spec *spec, int *len)
{
	char	*arg_str;
	void	*arg_ptr;

	*len = 0;
	if (spec->conv_id == STRING)
	{
		arg_str = va_arg(ap, char *);
		if (!arg_str)
		{
			spec->is_null = 1;
			return (base_s_str("(null)", len, spec));
		}
		return (base_s_str(arg_str, len, spec));
	}
	else if (spec->conv_id == POINTER)
	{
		arg_ptr = va_arg(ap, void *);
		if (!arg_ptr)
		{
			spec->is_null = 1;
			return (base_s_str("(nil)", len, spec));
		}
		return (base_s_ptr(arg_ptr, len));
	}
	return (NULL);
}

// Conv char : 'p' or 's'
// Specifiers to implement : [-], field width, precision (for 's' only)
// Method : first creates a string <base_s> containing the chars/digits,
// 		then applies precision for 's',
// 		then prints base_s and filler characters in the order given by [-]
int	printf_string(va_list ap, t_spec *spec)
{
	int		len;
	char	*base_s;

	base_s = base_s_strptr(ap, spec, &len);
	if (!base_s)
		return (len);
	if (spec->conv_id == STRING && spec->precision_defined
		&& spec->precision < len && !spec->is_null)
	{
		base_s[spec->precision] = '\0';
		len = spec->precision;
	}
	if (spec->flags[1])
	{
		ft_putstr_fd(base_s, 1);
		ft_putnchar_fd(' ', spec->fw - len, 1);
	}
	else
	{
		ft_putnchar_fd(' ', spec->fw - len, 1);
		ft_putstr_fd(base_s, 1);
	}
	free(base_s);
	return (ft_max(spec->fw, len));
}
