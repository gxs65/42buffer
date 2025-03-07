/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:21:43 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/09 14:29:12 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

char	*ft_strrchr(const char *string, int character)
{
	unsigned int	i;

	i = ft_strlen((char *)string);
	while (i != 0)
	{
		if (string[i] == (char)character)
			return ((char *)string + i);
		i--;
	}
	if (string[i] == (char)character)
		return ((char *)string + i);
	return (NULL);
}
