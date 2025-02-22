/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 15:16:22 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/12 13:20:19 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

char	*ft_strchr(const char *string, int character)
{
	unsigned int	i;

	i = 0;
	while (string[i])
	{
		if (string[i] == (char)character)
			return ((char *)string + i);
		i++;
	}
	if (string[i] == (char)character)
		return ((char *)(string + i));
	return (NULL);
}
