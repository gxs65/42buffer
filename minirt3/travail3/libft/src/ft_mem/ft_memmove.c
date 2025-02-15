/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 13:28:39 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/13 19:55:41 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*src_c;
	char	*dest_c;

	if (!dest && !src)
		return (NULL);
	src_c = (char *)src;
	dest_c = (char *)dest;
	if (dest <= src)
	{
		while (n--)
			*dest_c++ = *src_c++;
	}
	else
	{
		dest_c += n - 1;
		src_c += n - 1;
		while (n--)
			*dest_c-- = *src_c--;
	}
	return (dest);
}
