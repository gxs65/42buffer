/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:09:21 by aliutykh          #+#    #+#             */
/*   Updated: 2025/03/04 18:26:33 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (*str1 == *str2 && *str1 && *str2 && i < n)
	{
		i++;
		str1++;
		str2++;
	}
	if (i == n)
		return (0);
	return (*str1 - *str2);
}
int	ft_strcmp(char *s1, char *s2)
{
	int	ind;

	ind = 0;
	while (s1[ind] && s2[ind] && s1[ind] == s2[ind])
		ind++;
	return (s1[ind] - s2[ind]);
}