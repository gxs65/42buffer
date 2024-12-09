#include "ft_fractol.h"

void	log_imgdata(t_img *img)
{
	ft_printf(LOGS, "\n=== Image at %p ===\n", img->img_ptr);
	ft_printf(LOGS, "Pixel data at : %p\n", img->addr);
	ft_printf(LOGS, "Width, Height : %d-%d\n", img->width, img->height);
	ft_printf(LOGS, "Line length   : %d\n", img->line_length);
	ft_printf(LOGS, "Bits per pix  : %d\n", img->bpp);
	ft_printf(LOGS, "Bytes per pix : %d\n", img->opp);
	ft_printf(LOGS, "Endian        : %d\n", img->endian);
	ft_printf(LOGS, "======\n");
}

int	assemble_color(int a, int r, int g, int b)
{
	return (a * 0x01000000 + r * 0x010000 + g * 0x0100 + b * 0x01);
}
