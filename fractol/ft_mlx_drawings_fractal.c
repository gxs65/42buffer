#include "ft_fractol.h"

// Chooses the color of one point of the (conplex) plane
// 		- black if it belongs to the Julia set defined by constant <c>
// 		- otherwise, degrees of color according to the number of iterations
// 		  taken to diverge
void	draw_julia_point(t_img *img, t_point *p, t_point *c, double threshold)
{
	t_point	np;
	int		time;
	double	intensity;
	int		col;


	np.x = ((double)(p->x) * img->scale) / (double)(img->width) + img->nw_coords.x;
	np.y = ((double)(p->y) * img->scale) / (double)(img->width) + img->nw_coords.y;
	//printf("Point %f-%f after normalization : %f-%f\n", p->x, p->y, np.x, np.y); // LOG WITH PRINTF
	time = time_to_diverge(&np, c, threshold, MAX_ITER);
	//printf("\t\t-> gave time of %d\n", time); // LOG WITH PRINTF
	if (time == -1)
		change_pixel(img, p->x, p->y, assemble_color(0,0,0,0));
	else
	{
		intensity = sqrt((double)time / MAX_ITER);
		//printf("\t\t-> intensity %f\n", intensity); // LOG WITH PRINTF
		col = (int)(255 * intensity);
		change_pixel(img, p->x, p->y, assemble_color(0, col, col, col));
	}
}

// Draws a Julia set, by iterating on all the points of the (complex) plane
// to apply the simple quadratic map identified by complex constant <c>
// and determine wether the module diverges
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

// Chooses the color of one point of the (conplex) plane
// 		- black if it belongs to the Mandelbrot set
// 		- otherwise, degrees of color according to the number of iterations
// 		  taken to diverge
void	draw_mandelbrot_point(t_img *img, t_point *p, double threshold)
{
	t_point	nc;
	int		time;
	double	intensity;
	int		col;
	t_point	p0;

	p0.x = 0;
	p0.y = 0;
	nc.x = ((double)(p->x) * img->scale) / (double)(img->width) + img->nw_coords.x;
	nc.y = ((double)(p->y) * img->scale) / (double)(img->width) + img->nw_coords.y;
	printf("Point %f-%f after normalization : %f-%f\n", p->x, p->y, nc.x, nc.y); // LOG WITH PRINTF
	time = time_to_diverge(&p0, &nc, threshold, MAX_ITER);
	printf("\t\t-> gave time of %d\n", time); // LOG WITH PRINTF
	if (time == -1)
		change_pixel(img, p->x, p->y, assemble_color(0,0,0,0));
	else
	{
		intensity = sqrt((double)time / MAX_ITER);
		printf("\t\t-> intensity %f\n", intensity); // LOG WITH PRINTF
		col = (int)(255 * intensity);
		change_pixel(img, p->x, p->y, assemble_color(0, col, col, col));
	}
}

// Draws the Mandelbrot set, by iterating on the points of the (complex) plane
// to apply the simple quadratic map identified with starting point 0+0i
// qnd constant equal to the current point,
// and determine wether the module diverges
void	draw_mandelbrot(t_img *img, double threshold)
{
	t_point	p;

	printf("Drawing Mandelbrot set with threshold %f on img at %p\n", threshold, img->img_ptr); // LOG WITH PRINTF
	p.y = 0;
	while (p.y < img->height)
	{
		p.x = 0;
		while (p.x < img->width)
		{
			draw_mandelbrot_point(img, &p, threshold);
			p.x++;
		}
		p.y++;
	}
}
