/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 21:08:20 by abedin            #+#    #+#             */
/*   Updated: 2025/01/16 21:39:08 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

// Copies all <src> to the end of <dest>, no check of length
char	*ft_strcat(char *dest, const char *src)
{
	int		ind_src;
	int		ind_dest;

	ind_dest = 0;
	while (dest[ind_dest])
		ind_dest++;
	ind_src = 0;
	while (src[ind_src])
	{
		dest[ind_dest + ind_src] = src[ind_src];
		ind_src++;
	}
	dest[ind_dest + ind_src] = '\0';
	return (dest);
}

// Writes <n> times the character <c> at the end of <dest>,
// no check of length
char	*ft_strcat_nchar(char *dest, char c, int n)
{
	int	ind_dest;
	int	ind_n;

	ind_dest = 0;
	while (dest[ind_dest])
		ind_dest++;
	ind_n = 0;
	while (ind_n < n)
	{
		dest[ind_dest + ind_n] = c;
		ind_n++;
	}
	dest[ind_dest + ind_n] = '\0';
	return (dest);
}

// Fills the <size> bytes beginning at <s> with char value <filler>
void	ft_memset_asprt(void *s, int size, char filler)
{
	int		ind;
	char	*s2;

	s2 = (char *)s;
	ind = 0;
	while (ind < size)
	{
		s2[ind] = filler;
		ind++;
	}
}
