/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 08:55:34 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/12 13:06:30 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

char	*ft_strdup(const char *s)
{
	int			i;
	char		*dup;

	i = 0;
	dup = (char *)malloc((ft_strlen((char *)s) + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	while (s[i] != 0)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
