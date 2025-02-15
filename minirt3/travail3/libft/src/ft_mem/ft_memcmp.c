/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:16:33 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/09 13:49:56 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t				i;
	const unsigned char	*sc1;
	const unsigned char	*sc2;

	i = 0;
	sc1 = (const unsigned char *)s1;
	sc2 = (const unsigned char *)s2;
	while (i < n)
	{
		if (sc1[i] != sc2[i])
			return (sc1[i] - sc2[i]);
		i++;
	}
	return (0);
}
