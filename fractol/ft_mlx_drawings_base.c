#include "ft_fractol.h"

// Log line :
// ft_printf(LOGS, "Coloring pixel at %d-%d of img at %p with color %f\n", (int)(p->x), (int)(p->y), img->img_ptr, color);
void	change_pixel(t_img *img, int x, int y, int color)
{
	int	*pix;

	pix = (int *)img->addr;
	pix = pix + (y * img->width + x);
	*pix = color;
}

void	draw_circle(t_img *img, t_point *center, double radius, int color)
{
	int		x;
	int		y;
	double	limx;
	double	limy;
	t_point	p;

	ft_printf(LOGS, "Drawing circle with radius %d center %d-%d on img at %p\n", (int)radius, (int)(center->x), (int)(center->y), img->img_ptr);
	limy = center->y + radius;
	limx = center->x + radius;
	y = (int)(center->y - radius) - 1;
	while (y < limy)
	{
		x = (int)(center->x - radius);
		while (x < limx)
		{
			p.x = (double)x;
			p.y = (double)y;
			if (dist(&p, center) < radius)
				change_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

void	new_image(void *mlx, t_img *img, int width, int height)
{
	img->img_ptr = mlx_new_image(mlx, width, height);
	img->addr = mlx_get_data_addr(img->img_ptr, &(img->bpp), &(img->line_length), &(img->endian));
	img->opp = img->bpp / 8;
	img->width = width;
	img->height = height;
	img->scale = 3;
	img->nw_coords.x = 0 - (img->scale / 2);
	img->nw_coords.y = 0 - (img->scale / 2);
	ft_printf(LOGS, "New image created...\n");
	log_imgdata(img);
}