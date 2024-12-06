/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_fd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:26:36 by abedin            #+#    #+#             */
/*   Updated: 2024/11/19 10:26:38 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar_fd(char c, int fd)
{
	if (fd > 0)
		write(1, &c, 1);
}

int	ft_putnchar_fd(char c, int n, int fd)
{
	int	ind;

	ind = 0;
	while (ind < n)
	{
		ft_putchar_fd(c, fd);
		ind++;
	}
	return (n);
}

int	ft_putstr_fd(char *str, int fd)
{
	int	count;

	count = 0;
	while (str[count])
	{
		ft_putchar_fd(str[count], fd);
		count++;
	}
	return (count);
}
