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
# include <limits.h>

# define CHAR 0
# define STRING 1
# define POINTER 2
# define INTD 3
# define INTI 4
# define UINT 5
# define XUINT 6
# define XMAJUINT 7
# define PERCENT 8

typedef unsigned int	t_uint;
typedef unsigned long	t_ulong;

// Structure for storing printf specifiers after the '%' :
//		<flags>		: 0 or 1 for each of : 0 - + _ #
//		<fw>		: field width
//		<precision>	: precision (max #chars for str, min #digits for nbr)
//		<_defined>	: 0 or 1, storing if a '.' specifier was givem
//		<conv_id>	: 0 to 8 following the order in string : 'cspdiuxX%'
//		<step>		: 0 to 3 following the order : flags -> field width
//						-> precision ('.', then digits) -> conv identifier
// 		<is_null>   : 0 or 1, storing if the value to print is 0
//					  whatever the type
//					  (only there for conveniency of data transmission)
typedef struct s_spec {
	int	flags[5];
	int	fw;
	int	precision;
	int	precision_defined;
	int	conv_id;
	int	step;
	int	is_null;
}		t_spec;

// Utils
int		ft_max(int a, int b);
int		ft_min(int a, int b);
int		ft_strchrp(const char *s, char c);
size_t	ft_strlen(char *str);

// Level 0 : print only chars
void	ft_putchar_fd(char c, int fd);
int		ft_putnchar_fd(char c, int n, int fd);
int		ft_putstr_fd(char *str, int fd);

// Level 1 : convert integers/longs to strings 
int		ft_putnbr_base_count(long n, t_uint base);
int		ft_putnbr_base_in(long n, t_uint base, int _case, char **dest);
int		ft_putnbr_base_ulong_count(t_ulong n, t_uint base);
int		ft_putnbr_base_ulong_in(t_ulong n, t_uint base,
			int _case, char **dest);

// Helpers functions for level 2
int		print_extras(int is_neg, t_spec *spec, int fd);
void	apply_overrides(t_spec *spec);

// Level 2 : print according to specifiers
int		printf_char(va_list ap, t_spec *spec);
int		printf_string(va_list ap, t_spec *spec);
int		printf_percent(va_list ap, t_spec *spec);
int		printf_number(va_list ap, t_spec *spec);

// Storage of specifiers
// prototype of log function : `void	log_spec(t_spec *spec);`
void	init_spec(t_spec *spec);
void	store_valid_specifier(t_spec *spec, int cs_ind);
int		store_specifier(t_spec *spec, char c);

// Level 3 : printf
int		manage_notspec(char c, int *specifying);
int		manage_spec(char c, int *specifying, t_spec *spec, va_list ap);
int		printf_varg(t_spec *spec, va_list ap);
int		ft_printf(int mode, const char *s, ...);

#endif
