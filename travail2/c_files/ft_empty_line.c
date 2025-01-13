/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_empty_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 20:39:40 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/09 15:20:24 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/ft_minishell.h"

int	ft_empty_line(char *line)
{
	unsigned int	i;
	unsigned int	len;

	i = 0;
	len = 0;
	while (line[i] && ft_iswhitespace(line[i]))
	{
		i++;
		len++;
	}
	while (line[len])
		len++;
	if (i == len)
		return (1);
	return (0);
}
