/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_supp_log.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:20:58 by abedin            #+#    #+#             */
/*   Updated: 2025/01/29 16:20:59 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

// Displays strings stored in an array of size <len>
void	ft_display_string_array(char **array, int len)
{
	int	ind;

	ind = 0;
	while (ind < len)
	{
		ft_printf(LOGS, "'%s' ", array[ind]);
		ind++;
	}
}

// Displays ints stored in an array of size <len>
void	ft_display_int_array(char **array, int len)
{
	int	ind;

	ind = 0;
	while (ind < len)
	{
		ft_printf(LOGS, "'%d' ", array[ind]);
		ind++;
	}
}
