/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 20:40:18 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/13 17:39:09 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/ft_minishell.h"

int	ft_same_char(char first, char second);
int	ft_same_char(char first, char second)
{
	if (first == second)
		return (1);
	return (0);
}
