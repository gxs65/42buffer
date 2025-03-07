/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_in.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:26:46 by abedin            #+#    #+#             */
/*   Updated: 2025/01/15 22:41:16 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Functions for
//		- counting how many characters are needed in a string
//		  to be able to fully write the number into it (-> malloc)
// 		- printing the input in the malloc'd <dest> string of sufficient size
// The functions <put_..._in> work by manipulating a pointer to location
// 		to be written to in the string <dest> ;
// 			because that pointer must be modified by each recursion,
// 			the parameter is a pointer (to a pointer to char)

// For int numbers (specifiers 'diuxX') : <n> stored as long,
//		so as to manage INT MAX / INT MIN
// Parameter <_case> gives the case of letters for base > 10 :
// 		1 for uppercase, 0 for lowercase

int	ft_putnbr_base_count(long n, t_uint base)
{
	int	count;

	if (n < 0)
		return (1 + ft_putnbr_base_count(n * -1, base));
	else if (n < base)
		return (1);
	else
	{
		count = ft_putnbr_base_count(n / base, base);
		return (count + ft_putnbr_base_count(n % base, base));
	}
}

int	ft_putnbr_base_in(long n, t_uint base, int _case, char **dest)
{
	int	count;

	if (n < 0)
	{
		**dest = '-';
		(*dest)++;
		return (1 + ft_putnbr_base_in(n * -1, base, _case, dest));
	}
	else if (n < base)
	{
		if (n < 10)
			**dest = '0' + n;
		else
			**dest = 'a' - (32 * _case) + n - 10;
		(*dest)++;
		return (1);
	}
	else
	{
		count = ft_putnbr_base_in(n / base, base, _case, dest);
		return (count + ft_putnbr_base_in(n % base, base, _case, dest));
	}
}

// For unsigned long numbers (specifier 'p') : <n> stored as ul
// 		(since adresses can go up to ULONG_MAX)
int	ft_putnbr_base_ulong_count(t_ulong n, t_uint base)
{
	int	count;

	if (n < base)
		return (1);
	else
	{
		count = ft_putnbr_base_ulong_count(n / base, base);
		return (count + ft_putnbr_base_ulong_count(n % base, base));
	}
}

int	ft_putnbr_base_ulong_in(t_ulong n, t_uint base,
	int _case, char **dest)
{
	int	count;

	if (n < base)
	{
		if (n < 10)
			**dest = '0' + n;
		else
			**dest = 'a' - (32 * _case) + n - 10;
		(*dest)++;
		return (1);
	}
	else
	{
		count = ft_putnbr_base_ulong_in(n / base, base, _case, dest);
		return (count + ft_putnbr_base_ulong_in(n % base,
				base, _case, dest));
	}
}
