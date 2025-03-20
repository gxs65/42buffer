/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:24:43 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 21:07:57 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Functions hooking into MLX events

// Mouse events : for debug, launch and log a ray for given pixel
int	ft_handle_mouse(int button, int x, int y, t_data *data)
{
	(void)button;
	printf("mouse position : %d %d\n", x, y);
	ft_render_sample_one_pixel(data, x, y);
	return (0);
}

// Key events : not treated
int	ft_handle_keyrelease(int key, t_data *data)
{
	printf(">>> Key release %i\n", key);
	(void)data;
	return (0);
}

// Closes the window
int	ft_handle_destroy(t_data *data)
{
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	return (0);
}

// Handles MLX main loop
int	ft_handle_loop(t_data *data)
{
	(void)data;
	return (0);
}
