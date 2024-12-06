#include "ft_fractol.h"

void	draw_julia_point(t_img *img, t_point *p, t_point *c, double threshold)
{
	t_point	np;
	int		time;
	double	intensity;


	np.x = ((double)(p->x) * img->scale) / (double)(img->width) + img->nw_coords.x;
	np.y = ((double)(p->y) * img->scale) / (double)(img->width) + img->nw_coords.y;
	//printf("Point %d-%d after normalization : %f-%f\n", x, y, p.x, p.y); // LOG WITH PRINTF
	time = time_to_diverge(&np, c, threshold, MAX_ITER);
	//printf("\t\t-> gave time of %d\n", time); // LOG WITH PRINTF
	if (time == -1)
		change_pixel(img, p->x, p->y, assemble_color(0,0,0,0));
	else
	{
		intensity = sqrt((double)time / MAX_ITER);
		//printf("\t\t-> intensity %f\n", intensity); // LOG WITH PRINTF
		change_pixel(img, p->x, p->y, assemble_color(0, (int)(255 * intensity), 0, 0));
	}
}

void	draw_julia(t_img *img, t_point *c, double threshold)
{
	t_point	p;

	printf("Drawing Julia set with constant %f+%fi and threshold %f on img at %p\n", c->x, c->y, threshold, img->img_ptr); // LOG WITH PRINTF
	p.y = 0;
	while (p.y < img->height)
	{
		p.x = 0;
		while (p.x < img->width)
		{
			draw_julia_point(img, &p, c, threshold);
			p.x++;
		}
		p.y++;
	}
}
