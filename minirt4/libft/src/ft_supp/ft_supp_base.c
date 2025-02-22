/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_supp_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:20:39 by abedin            #+#    #+#             */
/*   Updated: 2025/01/29 16:20:43 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

// Computes the value (>= 0) in base 10
// contained in base 2 in the array <digits> of size <size>
unsigned int	to_base10(int *digits, int size, t_ui base)
{
	unsigned int	res;
	int				ind;

	ind = 0;
	res = 0;
	while (ind < size)
	{
		res = res * base + digits[ind];
		ind++;
	}
	return (res);
}

// Fills the array <digits> of size <size> with the digits
// of the value <n> written in base 2
// (returns 1 if <size> is too small to contain all digits)
int	from_base10(int *digits, int size, t_ui n, t_ui base)
{
	int	ind;

	ind = size - 1;
	while (n > 0 && ind >= 0)
	{
		digits[ind] = n % base;
		n = (n - n % base) / base;
		ind--;
	}
	if (n > 0)
		return (1);
	else
		return (0);
}

// Fills the array <tab> with the given value <val>
void	fill_tab_with(int *tab, int size, int val)
{
	int	ind;

	ind = 0;
	while (ind < size)
	{
		tab[ind] = val;
		ind++;
	}
}

// Returns the index of the minimum element in an int array
int	ind_max_in_tab(int *tab, int size)
{
	int	ind;
	int	ind_max;

	ind = 0;
	ind_max = 0;
	while (ind < size)
	{
		if (tab[ind] > tab[ind_max])
			ind_max = ind;
		ind++;
	}
	return (ind_max);
}

