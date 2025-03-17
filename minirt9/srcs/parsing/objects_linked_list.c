/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_linked_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:41:51 by abedin            #+#    #+#             */
/*   Updated: 2025/03/13 19:41:52 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_object	*ft_object_new()
{
	t_object	*res;

	res = malloc(1 * sizeof(t_object));
	if (!res)
		return (NULL);
	return (res);
}

void	ft_object_push_back(t_object **start, t_object *current)
{
	if (*start)
		current->next = *start;
	else
		current->next = NULL;
	*start = current;
}