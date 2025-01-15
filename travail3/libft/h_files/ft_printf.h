/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 14:18:14 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/07 12:23:43 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdint.h>
# include "libft.h"

//ft_printbasic.c
int		ft_printchar_fd(char c, int fd);
int		ft_printstr_fd(char *str, int fd);
int		ft_printnumber_fd(int n, int fd);
int		ft_printpercent_fd(int fd);

//ft_printpointer.c
int		ft_pointlen(uintptr_t decimal);
void	point_hex(uintptr_t num, int fd);
int		ft_printpoint_fd(uintptr_t addr, int fd);

//ft_printunsigned.c
int		ft_numlen(unsigned int n);
char	*ft_uitoa(unsigned int n);
int		ft_printunsigned_fd(unsigned int n, int fd);

//ft_printhexa.c
int		ft_hexlen(unsigned int decimal);
void	big_hex(unsigned int num, int fd);
void	lil_hex(unsigned int num, int fd);
int		ft_print_big_hex_fd(unsigned int num, int fd);
int		ft_print_lil_hex_fd(unsigned int num, int fd);

//ft_printf.c
int		ft_format_choice(va_list arg, char traveler, int fd);
int		ft_printf(int fd, const char *str, ...);

#endif
