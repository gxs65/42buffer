/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:03:30 by abedin            #+#    #+#             */
/*   Updated: 2024/11/15 13:03:32 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>

// Structure for storing printf specifiers after the '%' :
//		<flags>		: 0 or 1 for each of : 0 - + _ #
//		<fw>		: field width
//		<precision>	: precision (max #chars for str, min #digits for nbr)
//		<conv_id>	: 0 to 8 following the order : cspdiuxX%
//		<step>		: 0 to 4 following the order : flags -> field width
//						-> precision (.) -> -precision (digits) -> conversion
typedef struct s_spec {
	int	flags[5];
	int	fw;
	int	precision;
	int	conv_id;
	int	step;
}		t_spec;

int		ft_putnbr_base(long n, int base, int _case);
int		ft_putnbr_base_ulong(unsigned long n, unsigned long base, int _case);
int		ft_putstr(char *str);
void	ft_putchar(char c);

int		printf_p(va_list ap, char spec);
int		printf_i(va_list ap, char spec);
int		printf_u(va_list ap, char spec);

int		printf_c(va_list ap, char spec);
int		printf_s(va_list ap, char spec);
int		printf_percent(va_list ap, char spec);

int		ft_strchrp(const char *s, char c);
int		ft_printf(const char *s, ...);

int		store_specifier(t_spec *spec, char c);
int		printf_varg(t_spec *spec, va_list ap);

#endif
