/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printbasic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:03:52 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/20 13:01:46 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

// Prints the character given onto stdout (can be changed later) return length
int	ft_printchar_fd(char c, int fd)
{
	if (fd == -1)
		return (0);
	write(fd, &c, 1);
	return (1);
}

// Prints the content of str on stdout(can be changed later) returns the length
int	ft_printstr_fd(char *str, int fd)
{
	int	i;

	if (fd == -1)
		return (0);
	i = 0;
	if (str == NULL)
	{
		ft_putstr_fd("(null)", fd);
		return (6);
	}
	while (str[i])
		ft_printchar_fd(str[i++], fd);
	return (i);
}

// Prints the int given through str by itoa on stdout, returns the length.
int	ft_printnumber_fd(int n, int fd)
{
	int		length;
	char	*number_str;

	if (fd == -1)
		return (0);
	number_str = ft_itoa(n);
	if (!number_str)
		return (0);
	length = ft_printstr_fd(number_str, fd);
	free(number_str);
	return (length);
}

// Prints a percent sign and returns its length.
int	ft_printpercent_fd(int fd)
{
	if (fd == -1)
		return (0);
	ft_printchar_fd('%', fd);
	return (1);
}
