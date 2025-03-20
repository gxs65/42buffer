/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_logs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:56:19 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 21:00:37 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ft_log_intersection(t_object *object, t_intersection *intersect)
{
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURES] At intersection with object %p of type %d,\n",
			object, object->type);
		printf("\t\tcomputed : tangent - bitangent - normal :\n");
		ft_vec_display(intersect->tangent, 1);
		ft_vec_display(intersect->bitangent, 1);
		ft_vec_display(intersect->normal, 1);
	}
}
