/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:03:30 by abedin            #+#    #+#             */
/*   Updated: 2025/01/27 14:24:20 by administyra      ###   ########.fr       */
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
typedef struct s_spec
{
	int	flags[5];
	int	fw;
	int	precision;
	int	precision_defined;
	int	conv_id;
	int	step;
	int	is_null;
}		t_spec;

// Structure without any meaning, used uniquely
// to reduce the number of arguments to respect the Norm
typedef struct s_conveyer
{
	t_spec	*spec;
	char	*dest;
}				t_conveyer;

// Utils
int		ft_max(int a, int b);
int		ft_min(int a, int b);
int		ft_strchrp_asprt(const char *s, char c);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strcat_nchar(char *dest, char c, int n);
void	ft_memset_asprt(void *s, int size, char filler);

// Level 1 : convert integers/longs to strings 
int		ft_putnbr_base_count(long n, t_uint base);
int		ft_putnbr_base_in(long n, t_uint base, int _case, char **dest);
int		ft_putnbr_base_ulong_count(t_ulong n, t_uint base);
int		ft_putnbr_base_ulong_in(t_ulong n, t_uint base,
			int _case, char **dest);

// Helpers functions for level 2
int		print_extras(char *dest, int is_neg, t_spec *spec);
int		print_extras_count(int is_neg, t_spec *spec);
void	apply_overrides(t_spec *spec);

// Base string functions for level 2
char	*base_s_int(long n, t_spec *spec, int *len);
void	apply_precision(t_spec *spec, char *res, int *len, int nb_prec_digits);
long	retrieve_number(va_list ap, t_spec *spec);
char	*base_s_strptr(va_list ap, t_spec *spec, int *len);

// Level 2 : print according to specifiers
int		printf_char(t_conveyer *cv, va_list ap);
int		printf_string(t_conveyer *cv, va_list ap);
int		printf_percent(t_conveyer *cv, va_list ap);
int		printf_number(t_conveyer *cv, va_list ap);
int		printf_char_count(va_list ap, t_spec *spec);
int		printf_string_count(va_list ap, t_spec *spec);
int		printf_percent_count(va_list ap, t_spec *spec);
int		printf_number_count(va_list ap, t_spec *spec);

// Storage of specifiers for level 3
// prototype of log function : `void	log_spec(t_spec *spec);`
void	init_spec(t_spec *spec);
void	store_valid_specifier(t_spec *spec, int cs_ind);
int		store_specifier(t_spec *spec, char c);

// Level 3 : printf
int		manage_notspec(char *dest, char c, int *specifying);
int		manage_spec(t_conveyer *cv, char c, int *specifying, va_list ap);
int		manage_notspec_count(char c, int *specifying);
int		manage_spec_count(char c, int *specifying, t_spec *spec, va_list ap);
int		printf_varg(t_conveyer *cv, va_list ap);
int		printf_varg_count(t_spec *spec, va_list ap);
int		ft_printf(int fd, char *s, ...);
int		ft_printf_count(char *s, va_list ap);
int		ft_printf_store(char *dest, char *s, va_list ap);

#endif
