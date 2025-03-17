/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aliutykh <aliutykh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 16:04:00 by aliutykh          #+#    #+#             */
/*   Updated: 2024/05/21 15:14:11 by aliutykh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *substr, size_t len)
{
	size_t		i;
	size_t		j;

	if (len == 0 && !str)
		return (0);
	if (!*substr)
		return ((char *)str);
	i = 0;
	while (str[i] && i < len)
	{
		j = 0;
		while (substr[j] && str[i + j] == substr[j] && i + j < len)
			j++;
		if (!substr[j])
			return ((char *)&str[i]);
		i++;
	}
	return (0);
}
