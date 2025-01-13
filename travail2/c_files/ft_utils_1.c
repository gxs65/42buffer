/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 20:40:18 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/09 20:50:30 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/ft_minishell.h"

int	ft_isspace(char c);
int	ft_same_char(char first, char second);

int	ft_isspace(char c)
{
	if (c == ' ')
		return (1);
	return (0);
}

int	ft_same_char(char first, char second)
{
	if (first == second)
		return (1);
	return (0);
}
