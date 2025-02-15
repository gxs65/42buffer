/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 00:16:20 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/13 19:46:37 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (n != 0 && s1[i] != 0 && (s1[i] == s2[i]))
	{
		i++;
		n--;
	}
	if (n == 0)
		return (0);
	else
		return ((unsigned char)(s1[i]) - (unsigned char)(s2[i]));
}
