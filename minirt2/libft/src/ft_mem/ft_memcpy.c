/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:40:57 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/08 23:15:28 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*dst_cp;
	const char	*src_cp;

	if (dest == src || n == 0)
		return (dest);
	if (!dest && !src)
		return (0);
	dst_cp = (char *)dest;
	src_cp = (const char *)src;
	while (n--)
		dst_cp[n] = src_cp[n];
	return (dest);
}
