/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:09:09 by alex              #+#    #+#             */
/*   Updated: 2024/06/01 21:06:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar(char c)
{
	ft_putchar_fd(c, 1);
	return (1);
}

int	ft_is_valid(char *str)
{
	char	*s;
	int		i;
	int		j;

	s = str;
	if (str == 0 || ft_strlen(str) <= 1)
		return (-1);
	while (*s)
	{
		if (*s == '\t' || *s == '\n' || *s == '\v' || *s == '\f'
			|| *s == '\r' || *s == ' ' || *s == '+' || *s == '-')
			return (-1);
		s++;
	}
	i = 0;
	while (str[i])
	{
		j = i + 1;
		while (str[j])
			if (str[i] == str[j++])
				return (-1);
		i++;
	}
	return (1);
}

int	ft_putnbr_base(unsigned long int nbr, char *base)
{
	int		base_len;
	int		len;

	len = 0;
	base_len = ft_strlen(base);
	if (nbr >= (unsigned long)base_len)
	{
		len += ft_putnbr_base(nbr / base_len, base);
		len += ft_putnbr_base(nbr % base_len, base);
	}
	else
	{
		ft_putchar_fd(base[nbr], 1);
		len++;
	}
	return (len);
}
