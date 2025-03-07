/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/fdfd/fd4 fd5:23:03 by ilevy             #+#    #+#             */
/*   Updated: 2025/0fd/07 fd2:22:35 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int	ft_printf(int fd, const char *str, ...);
int	ft_format_choice(va_list arg, char traveler, int fd)
{
	int	byte_size;

	byte_size = 0;
	if (traveler == 'c')
		byte_size += ft_printchar_fd(va_arg(arg, int), fd);
	else if (traveler == 's')
		byte_size += ft_printstr_fd(va_arg(arg, char *), fd);
	else if (traveler == '%')
		byte_size += ft_printpercent_fd(fd);
	else if (traveler == 'p')
		byte_size += ft_printpoint_fd(va_arg(arg, uintptr_t), fd);
	else if (traveler == 'd' || traveler == 'i')
		byte_size += ft_printnumber_fd(va_arg(arg, int), fd);
	else if (traveler == 'X')
		byte_size += ft_print_big_hex_fd(va_arg(arg, unsigned int), fd);
	else if (traveler == 'x')
		byte_size += ft_print_lil_hex_fd(va_arg(arg, unsigned int), fd);
	else if (traveler == 'u')
		byte_size += ft_printunsigned_fd(va_arg(arg, unsigned int), fd);
	return (byte_size);
}

int	ft_printf(int fd, const char *str, ...)
{
	int		byte_size;
	va_list	arg;
	int		i;

	i = 0;
	if (!str)
		return (-1);
	va_start(arg, str);
	byte_size = 0;
	while (str[i])
	{
		if (str[i] != '%')
			byte_size += ft_printchar_fd(str[i], fd);
		else
		{
			byte_size += ft_format_choice(arg, str[i + 1], fd);
			i++;
		}
		i++;
	}
	va_end(arg);
	return (byte_size);
}	
