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

/*
Table of specifier validity :
???
Some specifiers override / deactivate other specifiers :
???
/!\ If there are invalid specifiers for a given conv_id,
???
*/

int	printf_notspec(char c, int *specifying)
{
	if (c == '%')
	{
		*specifying = 1;
		return (0);
	}
	else
	{
		//ft_putchar(c);
		return (1);
	}
}

int	printf_varg(t_spec *spec, va_list ap)
{
	log_spec(spec);
	(void)ap;
	return (0);
}

// Scans the input string and prints its characters one by one,
//		except when entering the "specifying" state after a '%' character :
// In this state it sends the chars to <store_specifier> to add them to <spec>,
//		- until the conversion char is found (-> print the arg using va_arg)
//		- or a char is an invalid specifier (-> UB, so here it just continues)
int	ft_printf(const char *s, ...)
{
	int		specifying;
	int		count;
	t_spec	spec;
	int		res_spec;
	va_list	ap;

	va_start(ap, s);
	init_spec(&spec);
	specifying = 0;
	count = 0;
	while (*s)
	{
		if (specifying)
		{
			res_spec = store_specifier(&spec, *s);
			if (res_spec == -1)
			{
				printf("[ft_printf] Invalid specifier char %c\n", *s);
				specifying = 0;
			}
			else if (res_spec == 1)
			{
				specifying = 0;
				count += printf_varg(&spec, ap);
				init_spec(&spec);
			}
		}
		else
			count += printf_easy(*s, &specifying);
		s++;
	}
	va_end(ap);
	return (count);
}
