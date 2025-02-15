/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_supp_itoa_in.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:20:55 by abedin            #+#    #+#             */
/*   Updated: 2025/01/29 16:20:55 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

// Recursively prints number <n> into string <s>
void	ft_itoa_in_recurr(int n, char *s, int ind)
{
	if (n < 10)
		s[ind] = '0' + n;
	else
	{
		ft_itoa_in_recurr(n / 10, s, ind - 1);
		ft_itoa_in_recurr(n % 10, s, ind);
	}
}

// Performs itoa on a string given as argument (no allocation)
// Must still calculate the number's size in the first place
// 		to give the correct starting index to <ft_itoa_in_recurr>
void	ft_itoa_in(int n, char *s)
{
	int	size;
	int	n_count;

	size = 0;
	if (n < 0)
	{
		s[0] = '-';
		n = n * -1;
		size++;
	}
	if (n == 0)
		size++;
	n_count = n;
	while (n_count > 0)
	{
		size++;
		n_count = n_count / 10;
	}
	s[size] = '\0';
	ft_itoa_in_recurr(n, s, size - 1);
}
