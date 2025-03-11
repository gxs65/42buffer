/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:26:11 by abedin            #+#    #+#             */
/*   Updated: 2025/01/16 21:47:57 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_max(int a, int b)
{
	if (a >= b)
		return (a);
	else
		return (b);
}

int	ft_min(int a, int b)
{
	if (a <= b)
		return (a);
	else
		return (b);
}

int	ft_strchrp_asprt(const char *s, char c)
{
	int	pos;

	pos = 0;
	while (s[pos])
	{
		if (s[pos] == (char)c)
			return (pos);
		pos++;
	}
	return (-1);
}
