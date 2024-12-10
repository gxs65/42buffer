#include "ft_fractol.h"

// Functions creating and using a <t_img> object,
// a structure containing two "MLX images" to avoid screed tearing :
// one image is displayed (display image),
// while the other is being worked on / modified (work image)

// Initializes all the variables contained in the <t_img> structure, notably
//  	- <img_caches> the pointers to the 2 images
// 		- <addr> the pointers to the pixel descriptions of the 2 images
// 		- <which_img> the index (0 or 1) of the work image
// (see header for funer description of all variables)
void	new_image(void *mlx, t_img *img, int width, int height)
{
	img->img_caches[0] = mlx_new_image(mlx, width, height);
	img->addr[0] = mlx_get_data_addr(img->img_caches[0], &(img->bpp), &(img->line_length), &(img->endian));
	img->img_caches[1] = mlx_new_image(mlx, width, height);
	img->addr[1] = mlx_get_data_addr(img->img_caches[1], &(img->bpp), &(img->line_length), &(img->endian));
	img->which_img = 0;
	img->img_ptr = img->img_caches[img->which_img];
	img->opp = img->bpp / 8;
	img->width = width;
	img->height = height;
	img->scale = 4;
	img->nw_coords.x = 0 - (img->scale / 2);
	img->nw_coords.y = 0 - (img->scale / 2) * (img->height / img->width);
	img->depth = MAX_ITER;
	ft_printf(LOGS, "New image created...\n");
	log_imgdata(img);
}

// Swaps the two images contained by a <t_img> structure :
// 		- work image becomes display image (with MLX function <put_image>)
// 		- display image becomes work image (without reinitializing it)
void	display_changed_image(t_mlxdata *data, t_img *img)
{
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, img->img_ptr, 0, 0);
	img->which_img = 1 - img->which_img;
	img->img_ptr = img->img_caches[img->which_img];
}

// Changes the color of the pixel at the given position
// 		/!\ the pixel is changed on the work image, not the display image,
// 			so <display_new_image> must be called before changes are visible
void	change_pixel(t_img *img, int x, int y, int color)
{
	int	*pix;

	pix = (int *)img->addr[img->which_img];
	pix = pix + (y * img->width + x);
	*pix = color;
}

// Fills an image with black pixels
void	draw_black(t_img *img)
{
	t_point	p;

	p.y = 0;
	while (p.y < img->height)
	{
		p.x = 0;
		while (p.x < img->width)
		{
			change_pixel(img, p.x, p.y, assemble_color(0, 0, 0, 0));
			p.x++;
		}
		p.y++;
	}
}