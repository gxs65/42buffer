#include "ft_fractol.h"

// Log function for informations stored in <t_ing> structures
void	log_imgdata(t_img *img)
{
	ft_printf(LOGS, "\n=== Image at %p | %p ===\n",
		img->img_caches[img->which_img], img->img_caches[1 - img->which_img]);
	ft_printf(LOGS, "Pixel data at  : %p | %p\n",
		img->addr[img->which_img], img->addr[1 - img->which_img]);
	ft_printf(LOGS, "Width, Height  : %d-%d\n", img->width, img->height);
	ft_printf(LOGS, "Line length    : %d\n", img->line_length);
	ft_printf(LOGS, "Bits per pix   : %d\n", img->bpp);
	ft_printf(LOGS, "Bytes per pix  : %d\n", img->opp);
	ft_printf(LOGS, "Endian         : %d\n", img->endian);
	ft_printf(LOGS, "Depth          : %d\n", img->depth);
	printf("NW coordinates : %f'%f\n", img->nw_coords.x, img->nw_coords.y); // LOGS WITH PRINTF
	printf("Scale          : %f\n", img->scale); // LOGS WITH PRINTF
	ft_printf(LOGS, "======\n");
}

// Created a single 4-bytes integer (int)
// out of the 4 intensities (0-255) of ARGB components
int	assemble_color(int a, int r, int g, int b)
{
	int color;

	color = (a * 0x01000000 + r * 0x010000 + g * 0x0100 + b * 0x01);
	return (color);
}

// Allocates a 2-dimensional array of chars, and fills it with zeroes
char	**allocate_chararray_2dim(int width, int height)
{
	char	**res;
	int		indy;
	int		indx;

	res = malloc(height * sizeof(char *));
	indy = 0;
	while (indy < height)
	{
		res[indy] = malloc(width * sizeof(char));
		indx = 0;
		while (indx < width)
		{
			res[indy][indx] = 0;
			indx++;
		}
		indy++;
	}
	return (res);
}

// Appreciates the position of the given point on the image
// 		- return value 0 : point inside the image
// 		- return value 1 : point on the edge of image
// 		- return value 2 : point outside image
int	is_on_edge(t_img *img, int x, int y)
{
	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return (2);
	else if (x == 0 || x == img->width - 1 || y == 0 || y >= img->height - 1)
		return (1);
	else
		return (0);
}