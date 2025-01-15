/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 16:37:23 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/08 23:01:32 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int	ft_atoi(const char *nptr)
{
	size_t	i;
	size_t	n;
	int		sign;

	i = 0;
	sign = 1;
	n = 0;
	while (nptr[i] == ' ' || (nptr[i] >= 9 && nptr[i] <= 13))
		i++;
	while (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -1;
		if (nptr[i + 1] == '-' || nptr[i + 1] == '+')
			return (0);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		n = n * 10 + (nptr[i] - '0');
		i++;
	}
	n = n * sign;
	return (n);
}
