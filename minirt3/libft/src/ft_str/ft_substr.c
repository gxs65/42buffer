/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 09:06:43 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/12 13:06:23 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char			*dup;
	unsigned int	i;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= (unsigned int)ft_strlen((char *)s))
		return (ft_strdup(""));
	if (len > (unsigned int)ft_strlen((char *)s) - start)
		len = (unsigned int)ft_strlen((char *)s) - start;
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	while (i < len && s[start + i] != '\0')
	{
		dup[i] = s[start + i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
