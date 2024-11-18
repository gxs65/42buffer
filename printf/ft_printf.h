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
//		<conv_id>	: 0 to 8 following the order in string : 'cspdiuxX%'
//		<step>		: 0 to 3 following the order : flags -> field width
//						-> precision ('.', then digits) -> conv identifier
typedef struct s_spec {
	int	flags[5];
	int	fw;
	int	precision;
	int	conv_id;
	int	step;
}		t_spec;

// Utils
int		ft_max(int a, int b);
int		ft_min(int a, int b);
int		ft_strchrp(const char *s, char c);

// Level 0 : print only chars and digits
int		ft_putnbr_base(long n, int base, int _case);
int		ft_putnbr_base_ulong(unsigned long n, unsigned long base, int _case);
int		ft_putstr(char *str);
void	ft_putchar(char c);
void	ft_putnchar(char c, int n);

// Level 1 : print according to specifiers
int		printf_char(va_list ap, char spec);
int		printf_string(va_list ap, char spec);
int		printf_percent(va_list ap, char spec);
int		printf_number(va_list ap, char spec);

// Storage of specifiers
void	init_spec(t_spec *spec);
void	store_valid_specifier(t_spec *spec, int cs_ind);
int		store_specifier(t_spec *spec, char c);

// Level 2 : printf
int		printf_notspec(char c, int *specifying);
int		printf_varg(t_spec *spec, va_list ap);
int		ft_printf(const char *s, ...);

#endif
