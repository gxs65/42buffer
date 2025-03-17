/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_to_int.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:24:20 by abedin            #+#    #+#             */
/*   Updated: 2025/03/13 19:25:51 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Creates a single 4-bytes integer (int)
// out of the 4 intensities (0-255) of ARGB components
int	ft_assemble_color(t_vec color_components)
{
	int	color;
	int	a;
	int	r;
	int	g;
	int	b;

	if (color_components.x > 1)
		color_components.x = 1;
	if (color_components.y > 1)
		color_components.y = 1;
	if (color_components.z > 1)
		color_components.z = 1;

	a = 0;
	r = (int)(255 * sqrt(ft_fabs(color_components.x)));
	g = (int)(255 * sqrt(ft_fabs(color_components.y)));
	b = (int)(255 * sqrt(ft_fabs(color_components.z)));
	
	/*
	r = (int)(255 * (color_components.x));
	g = (int)(255 * (color_components.y));
	b = (int)(255 * (color_components.z));
	*/
	color = (a * 0x01000000 + r * 0x010000 + g * 0x0100 + b * 0x01);
	return (color);
}
