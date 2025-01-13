/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printpointer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:03:36 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/20 12:54:42 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int		ft_pointlen(uintptr_t decimal);
void	point_hex(uintptr_t num, int fd);
int		ft_printpoint_fd(uintptr_t addr, int fd);

int	ft_pointlen(uintptr_t decimal)
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

void	point_hex(uintptr_t num, int fd)
{
	if (num >= 16)
	{
		point_hex(num / 16, fd);
		point_hex(num % 16, fd);
	}
	else
	{
		if (num <= 9)
			ft_printchar_fd((num + '0'), fd);
		else
			ft_printchar_fd((num - 10 + 'a'), fd);
	}
}

int	ft_printpoint_fd(uintptr_t addr, int fd)
{
	int			byte_size;

	byte_size = 0;
	if (fd == -1)
		return (0);
	if (addr == 0)
		byte_size += ft_printstr_fd("(nil)", fd);
	else
	{
		byte_size += ft_printstr_fd("0x", 1);
		point_hex(addr, fd);
		byte_size += ft_pointlen(addr);
	}
	return (byte_size);
}
