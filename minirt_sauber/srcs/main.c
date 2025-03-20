/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:35:28 by alex              #+#    #+#             */
/*   Updated: 2025/03/20 21:33:47 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	g_log;

void		ft_free_mlx_base(t_data *data);
int			ft_init_mlx(t_data *data, t_img *img);

// Renders the ray-traced image, then displays it in the window
void	ft_draw(t_data *data)
{
	printf("Drawing...\n");
	ft_render(data);
	printf("Displaying...\n");
	ft_display_changed_image(data, data->img);
}

// Main : executes steps
//		- parse input file
// 		- load textures
// 		- start MLX
// 		- draw image
// 		- go into debug mode
int	main(int ac, char **av)
{
	t_data		data;
	t_img		img;

	if (ac != 2)
		return (1);
	data.img = &img;
	img.data = &data;
	g_log = 0;
	if (ft_init_parsing(&data, av))
		return (1);
	if (ft_init_textures(&data))
		return (1);
	ft_log_scene(&data);
	ft_log_textures(data.textures, data.nb_textures);
	if (ft_init_mlx(&data, &img))
		return (1);
	ft_draw(&data);
	g_log = B_COLORING | B_INTERSECTION | B_SHADOWS | B_TEXTURES;
	mlx_loop(data.mlx_ptr);
	ft_free_mlx_base(&data);
	return (0);
}
