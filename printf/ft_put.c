/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:02:57 by abedin            #+#    #+#             */
/*   Updated: 2024/11/15 13:02:59 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int	ft_putstr(char *str)
{
	int	count;

	count = 0;
	while (str[count])
	{
		ft_putchar(str[count]);
		count++;
	}
	return (count);
}

// The conv identifiers to implement (diuxX) take only int or uint arguments,
//		-> MAX / MIN managed by manipulating a long variable
// Parameter <_case> gives the case of letters for base > 10 :
// 		1 for uppercase, 0 for lowercase
int	ft_putnbr_base(long n, int base, int _case)
{
	int	count;

	if (n < 0)
	{
		ft_putchar('-');
		return (1 + ft_putnbr_base(n * -1, base, _case));
	}
	else if (n < base)
	{
		if (n < 10)
			ft_putchar('0' + n);
		else
			ft_putchar('a' - (32 * _case) + n - 10);
		return (1);
	}
	else
	{
		count = ft_putnbr_base(n / base, base, _case);
		return (count + ft_putnbr_base(n % base, base, _case));
	}
}

// The conv identifiers 'p' requires to print an unsigned long number,
// 		-> since there is no bigger variable type,
//			necessary to have a specific function for ulong
int	ft_putnbr_base_ulong(unsigned long n, unsigned long base, int _case)
{
	int	count;

	if (n < base)
	{
		if (n < 10)
			ft_putchar('0' + n);
		else
			ft_putchar('a' - (32 * _case) + n - 10);
		return (1);
	}
	else
	{
		count = ft_putnbr_base_ulong(n / base, base, _case);
		return (count + ft_putnbr_base_ulong(n % base, base, _case));
	}
}
