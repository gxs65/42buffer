/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_image_swap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:25:39 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:30:28 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Functions creating and using a <t_img> object,
// a structure containing two "MLX images" to avoid screed tearing :
// one image is displayed (display image),
// while the other is being worked on / modified (work image)

// Initializes common variables for <t_img> structures, notably
//  	- <img_caches> the pointers to the 2 images
// 		- <addr> the pointers to the pixel descriptions of the 2 images
// 		- <which_img> the index (0 or 1) of the work image
// (see header for further description of all variables)
// Returns 1 if a malloc failed
int	ft_new_image(t_data *data, t_img *img, int width, int height)
{
	img->img_caches[0] = mlx_new_image(data->mlx_ptr, width, height);
	img->img_caches[1] = mlx_new_image(data->mlx_ptr, width, height);
	if (!img->img_caches[0] || !img->img_caches[1])
		return (1);
	img->addr[0] = mlx_get_data_addr(img->img_caches[0], &(img->bpp),
			&(img->line_length), &(img->endian));
	img->addr[1] = mlx_get_data_addr(img->img_caches[1], &(img->bpp),
			&(img->line_length), &(img->endian));
	img->which_img = 0;
	img->img_ptr = img->img_caches[img->which_img];
	img->opp = img->bpp / 8;
	img->width = width;
	img->height = height;
	printf("New MLX image created...\n");
	return (0);
}

// Swaps the two images contained by a <t_img> structure :
// 		- work image becomes display image (with MLX function <put_image>)
// 		- display image becomes work image (without reinitializing it)
void	ft_display_changed_image(t_data *data, t_img *img)
{
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, img->img_ptr, 0, 0);
	img->which_img = 1 - img->which_img;
	img->img_ptr = img->img_caches[img->which_img];
}

// Changes the color of the pixel at the given position
// 		/!\ the pixel is changed on the work image, not the display image,
// 			so <display_new_image> must be called before changes are visible
void	ft_change_pixel(t_img *img, int x, int y, int color)
{
	int	*pix;

	pix = (int *)img->addr[img->which_img];
	pix = pix + (y * img->width + x);
	*pix = color;
}
