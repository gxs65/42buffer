/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printunsigned.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:58:46 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/16 21:25:49 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int	ft_numlen(unsigned int n)
{
	int	len;

	len = 0;
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_uitoa(unsigned int n)
{
	char	*numstr;
	int		len;

	len = ft_numlen(n);
	numstr = (char *)malloc((len + 1) * sizeof(char));
	if (!numstr)
		return (0);
	numstr[len--] = '\0';
	while (n)
	{
		numstr[len--] = n % 10 + 48;
		n /= 10;
	}
	return (numstr);
}

int	ft_printunsigned_fd(unsigned int n, int fd)
{
	int		byte_size;
	char	*numstr;

	if (fd == -1)
		return (0);
	byte_size = 0;
	if (n == 0)
		byte_size += write(1, "0", fd);
	else
	{
		numstr = ft_uitoa(n);
		byte_size += ft_printstr_fd(numstr, fd);
		free(numstr);
	}
	return (byte_size);
}
