/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:27:32 by abedin            #+#    #+#             */
/*   Updated: 2024/11/15 12:29:33 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_strchrp(const char *s, char c)
{
	int	pos;

	pos = 0;
	while (s[pos])
	{
		if (s[pos] == (char)c)
			return (pos);
		pos++;
	}
	return (0);
}

int	printf_easy(char c, int *specifying)
{
	if (c == '%')
	{
		*specifying = 1;
		return (0);
	}
	else
	{
		ft_putchar(c);
		return (1);
	}
}

// Function verifying if the given character is a valid specifier
//		depending on the step reached by spec->step,
//		eg. a flag (step 0) can't follow the precision specification (step 3)
// -> return value :
//		. 0 : the specifier is valid, we can continue scanning the input string
//		. 1 : the specifier is the conversion character, we can print the arg
//		. -1 : the specifier is invalid
int		store_specifier(t_spec *spec, char c);


// Scans the input string and prints its characters one by one,
//		except when entering the "specifying" state after a '%' character :
//		then it sends the chars to <store_specifier> to add them to <spec>,
//		until the conversion char is found (-> print the arg zwith va_arg)
//		or a char is an invalid specifier (-> UB, so here it just continues)
int	ft_printf(const char *s, ...)
{
	int		specifying;
	int		count;
	t_spec	*spec;
	int		res_spec;
	va_list	ap;

	va_start(ap, s);
	init_spec(spec);
	specifying = 0;
	count = 0;
	while (*s)
	{
		if (specifying)
		{
			res_spec = store_specifier(spec, *s);
			if (res_spec = -1)
				specifying = 0;
			else if (res_spec == 1)
			{
				specifying = 0;
				count += printf_varg(spec, ap);
				init_spec(spec);
			}
		}
		else
			count += printf_easy(*s, &specifying);
		s++;
	}
	va_end(ap);
	return (count);
}
