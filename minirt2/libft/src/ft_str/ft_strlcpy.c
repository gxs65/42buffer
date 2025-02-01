/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 20:06:31 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/12 13:19:26 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int	ft_strlcpy(char *dest, const char *src, unsigned int size)
{
	unsigned int	dst_length;
	unsigned int	src_length;

	dst_length = 0;
	src_length = 0;
	while (src[src_length])
		src_length++;
	if (size == 0)
		return (src_length);
	while (src[dst_length] != 0 && dst_length < size - 1)
	{
		dest[dst_length] = src[dst_length];
		dst_length++;
	}
	dest[dst_length] = '\0';
	return (src_length);
}
