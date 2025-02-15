/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 09:13:19 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/08 23:34:58 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	unsigned int	i;
	unsigned int	j;
	unsigned int	index;
	char			*join;

	i = ft_strlen((char *) s1);
	j = ft_strlen((char *) s2);
	index = 0;
	join = (char *)malloc((i + j + 1) * sizeof(char));
	if (!join)
		return (0);
	while (index < i)
	{
		join[index] = s1[index];
		index++;
	}
	index = 0;
	while (index < j)
	{
		join[index + i] = s2[index];
		index++;
	}
	join[index + i] = 0;
	return (join);
}
