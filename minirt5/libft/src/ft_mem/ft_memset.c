/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 18:48:15 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/13 19:51:59 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	length;

	length = 0;
	while (length < n)
	{
		*(unsigned char *)(s + length) = (unsigned char) c;
		length++;
	}
	return (s);
}
