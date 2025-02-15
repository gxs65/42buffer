/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 09:48:05 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/12 13:22:10 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*cameleon;
	size_t	total;

	total = size * count;
	if (count && total / count != size)
		return (NULL);
	cameleon = (void *)malloc(total);
	if (cameleon == NULL)
		return (NULL);
	ft_bzero(cameleon, total);
	return (cameleon);
}
