/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:56:58 by alex              #+#    #+#             */
/*   Updated: 2024/06/01 21:07:35 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stdio.h>
# include "libft.h"

int		ft_printf(const char *str, ...);
int		ft_parse(const char *format, va_list args);

int		ft_putnbr_base(unsigned long int nbr, char *base);
int		ft_putchar(char c);

int		ft_parse_str(va_list args);
int		ft_parse_pointer(va_list args);
int		ft_parse_int(va_list args);
int		ft_parse_nbase(va_list args, char *base);

#endif