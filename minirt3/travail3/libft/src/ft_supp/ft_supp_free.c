/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_supp_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 16:20:47 by abedin            #+#    #+#             */
/*   Updated: 2025/01/29 16:20:48 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

// Frees an array of strings, up to and not including index <ind>
// Always returns 1
int	ft_free_strs_tab_upto(char **tab, int lim)
{
	int	ind;

	ind = 0;
	while (ind < lim)
	{
		free(tab[ind]);
		ind++;
	}
	free(tab);
	return (1);
}

// Frees everything in a null-terminated array of strings
// Always returns 1
int	ft_free_strs_tab_nullterm(char **tab)
{
	int	ind;

	ind = 0;
	while (tab[ind])
	{
		free(tab[ind]);
		ind++;
	}
	free(tab);
	return (1);
}
