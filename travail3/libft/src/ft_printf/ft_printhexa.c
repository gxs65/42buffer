/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhexa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:31:41 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/20 13:03:33 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int		ft_hexlen(unsigned int decimal);
void	big_hex(unsigned int num, int fd);
void	lil_hex(unsigned int num, int fd);
int		ft_print_big_hex_fd(unsigned int num, int fd);
int		ft_print_lil_hex_fd(unsigned int num, int fd);

int	ft_hexlen(unsigned int decimal)
{
	int	length_hex;

	if (!decimal)
		return (1);
	length_hex = 0;
	while (decimal)
	{
		length_hex++;
		decimal /= 16;
	}
	return (length_hex);
}

void	big_hex(unsigned int num, int fd)
{
	if (num >= 16)
	{
		big_hex(num / 16, fd);
		big_hex(num % 16, fd);
	}
	else
	{
		if (num <= 9)
			ft_printchar_fd((num + '0'), fd);
		else
			ft_printchar_fd((num - 10 + 'A'), fd);
	}
}

void	lil_hex(unsigned int num, int fd)
{
	if (num >= 16)
	{
		lil_hex(num / 16, fd);
		lil_hex(num % 16, fd);
	}
	else
	{
		if (num <= 9)
			ft_printchar_fd((num + '0'), fd);
		else
			ft_printchar_fd((num - 10 + 'a'), fd);
	}
}

int	ft_print_big_hex_fd(unsigned int num, int fd)
{
	int	byte_size;

	if (fd == -1)
		return (0);
	byte_size = 0;
	if (num == 0)
		byte_size += ft_printchar_fd('0', fd);
	else
	{
		byte_size += ft_hexlen(num);
		big_hex(num, fd);
	}
	return (byte_size);
}

int	ft_print_lil_hex_fd(unsigned int num, int fd)
{
	int	byte_size;

	if (fd == -1)
		return (0);
	byte_size = 0;
	if (num == 0)
		byte_size += ft_printchar_fd('0', fd);
	else
	{
		byte_size += ft_hexlen(num);
		lil_hex(num, fd);
	}
	return (byte_size);
}
