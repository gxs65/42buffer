/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:47:00 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/09 00:17:10 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

char	*ft_strnstr(const char *big, const char *little, unsigned int len)
{
	unsigned int	i;
	unsigned int	c;
	unsigned int	n_len;
	char			*bigc;

	i = 0;
	bigc = (char *)big;
	n_len = ft_strlen((char *)little);
	if (n_len == 0 || little == big)
		return (bigc);
	while (bigc[i] && i < len)
	{
		c = 0;
		while (bigc[i + c] && little[c] && bigc[i + c] == little[c]
			&& i + c < len)
			c++;
		if (c == n_len)
			return (bigc + i);
		i++;
	}
	return (0);
}
