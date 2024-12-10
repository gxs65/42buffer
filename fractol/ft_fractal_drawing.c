#include "ft_fractol.h"

// For one pixel on the image of a fractal
// 		- calculates the corresponding coordinates on the complex plane
// 		- calls <time_to_diverge> with the correct paras according to ftype
// 			-> drawing Julia (0) : z0 is the candidate, c is user-defined
// 			-> drawing Mandelbrot : z0 is 0+0i, c is the candidate
// 		- colors the pixel according to the time taken to diverge
// 		- returns this time
int	draw_one_point(t_img *img, t_point *p, int recurr_lvl)
{
	t_point	candidate;
	t_point	p0;
	int		time;
	int		intensity;

	(void)recurr_lvl;
	candidate.x = ((double)(p->x) * img->scale) / (double)(img->width) + img->nw_coords.x;
	candidate.y = ((double)(p->y) * img->scale) / (double)(img->width) + img->nw_coords.y;
	if (img->ftype == 0)
		time = time_to_diverge(&candidate, &(img->c), img->threshold_sq, img->depth);
	else
	{
		p0.x = 0;
		p0.y = 0;
		time = time_to_diverge(&p0, &candidate, img->threshold_sq, img->depth);
	}
	//printf("\tPoint %f'%f after normalization : %f'%f\n", p->x, p->y, nc.x, nc.y); // LOG WITH PRINTF
	//printf("\t\t-> gave time of %d\n", time); // LOG WITH PRINTF
	if (time == -1)
		change_pixel(img, p->x, p->y, assemble_color(0, 0, 0, 0));
	else
	{
		intensity = (int)(255 * sqrt((double)time / img->depth));
		change_pixel(img, p->x, p->y, assemble_color(0, intensity, intensity, intensity));
	}
	return (time);
}

// Drawing Mandelbrot : escape time + iteration on each point of the plane

// Draws the Mandelbrot set, by iterating on the points of the (complex) plane
// to apply the simple quadratic map with constant equal to the current point,
// and determine wether the module diverges
void	draw_mandelbrot1(t_img *img)
{
	t_point	p;

	printf("[Method 1] Drawing Mandelbrot set with threshold %f on img at %p, NW coords = %f'%f\n",
		img->threshold_sq, img->img_ptr, img->nw_coords.x, img->nw_coords.y); // LOG WITH PRINTF
	p.y = 0;
	while (p.y < img->height)
	{
		p.x = 0;
		while (p.x < img->width)
		{
			draw_one_point(img, &p, 0);
			p.x++;
		}
		p.y++;
	}
}

// Drawing Mandelbrot : escape time + recursive explo to avoid cardioid

// Determines if the given point produces a diverging suite
// and colors it accordingly (through <draw_one_point>),
// then explores the neighbor points if it was not elaready explored, and
// 		- either it is on the edge of the image
// 		- or it is within the image, and the current point diverged
// (this ensures that the interior of the mandelbrot set is never explored)
void	explore_mandelbrot_point(t_img *img, int x, int y, char **explored, int recurr_lvl, int *nb_pixels)
{
	t_point	p;
	int 	time;
	int		on_edge;

	//ft_printf(LOGS, "Exploring point at %d'%d\n", x, y);
	explored[y][x] = 1;
	(*nb_pixels)++;
	p.x = x;
	p.y = y;
	time = draw_one_point(img, &p, recurr_lvl);

	on_edge = is_on_edge(img, x + 1, y);
	if (on_edge != 2 && !explored[y][x + 1] && (on_edge == 1 || (time != -1 && on_edge == 0)))
		explore_mandelbrot_point(img, x + 1, y, explored, recurr_lvl + 1, nb_pixels);
	on_edge = is_on_edge(img, x - 1, y);
	if (on_edge != 2 && !explored[y][x - 1] && (on_edge == 1 || (time != -1 && on_edge == 0)))
		explore_mandelbrot_point(img, x - 1, y, explored, recurr_lvl + 1, nb_pixels);
	on_edge = is_on_edge(img, x, y + 1);
	if (on_edge != 2 && !explored[y + 1][x] && (on_edge == 1 || (time != -1 && on_edge == 0)))
		explore_mandelbrot_point(img, x, y + 1, explored, recurr_lvl + 1, nb_pixels);
	on_edge = is_on_edge(img, x, y - 1);
	if (on_edge != 2 && !explored[y - 1][x] && (on_edge == 1 || (time != -1 && on_edge == 0)))
		explore_mandelbrot_point(img, x, y - 1, explored, recurr_lvl + 1, nb_pixels);
}

// Draws the Mandelbrot set, by exploring recursively the points of the plane
// to apply the simple quadratic map with constant equal to the current point,
// and determine wether the module diverges
// 		-> prunes exploration at non-diverging points, to follow the border
// 		   of the Mandelbrot set, instead of entering it
void	draw_mandelbrot2(t_img *img)
{
	char	**explored;
	int		nb_pixels;

	printf("[Method 2] Drawing Mandelbrot set with threshold %f on img at %p, NW coords = %f'%f\n",
		img->threshold_sq, img->img_ptr, img->nw_coords.x, img->nw_coords.y); // LOG WITH PRINTF
	draw_black(img);
	nb_pixels = 0;
	explored = allocate_chararray_2dim(img->width, img->height);
	explore_mandelbrot_point(img, 0, 0, explored, 0, &nb_pixels);
	ft_printf(LOGS, "\texplored and rendered pixels : %d\n", nb_pixels);
}

// Drawing Mandelbrot : escape time + recursive explo for border rendering

t_rlist	*create_queue(int width, int height, int **in_queue, int **times)
{
	t_rlist	*queue;
	t_rlist	*newelem;
	int		ind;

	ind = 0;
	queue = NULL;
	while (ind < width)
	{
		newelem = ft_lstnew(ind, 0);
		ft_lstadd_back(&queue, newelem);
		times[0][ind] = -2;
		in_queue[0][ind] = 1;
		newelem = ft_lstnew(ind, height - 1);
		ft_lstadd_back(&queue, newelem);
		times[height - 1][ind] = -2;
		in_queue[height - 1][ind] = 1;
		ind++;
	}
	ind = 0;
	while (ind < height)
	{
		newelem = ft_lstnew(0, ind);
		ft_lstadd_back(&queue, newelem);
		times[ind][0] = -2;
		in_queue[ind][0] = 1;
		newelem = ft_lstnew(width - 1, ind);
		ft_lstadd_back(&queue, newelem);
		times[ind][width - 1] = -2;
		in_queue[ind][width - 1] = 1;
		ind++;
	}
	return (queue);
}

int	**allocate_intarray_2dim(int width, int height)
{
	int		**res;
	int		indy;
	int		indx;

	res = malloc(height * sizeof(int *));
	indy = 0;
	while (indy < height)
	{
		res[indy] = malloc(width * sizeof(int));
		indx = 0;
		while (indx < width)
		{
			res[indy][indx] = -1;
			indx++;
		}
		indy++;
	}
	return (res);
}

int	explore_one_point(int x, int y, t_img *img, int **times)
{
	t_point	p;

	if (times[y][x] == -2)
	{
		p.x = (double)x;
		p.y = (double)y;
		times[y][x] = draw_one_point(img, &p, 0);
	}
	return (times[y][x]);
}

int	add_to_queue(int x, int y, t_rlist **queue, int **in_queue)
{
	t_rlist	*newelem;

	newelem = ft_lstnew(x, y);
	ft_lstadd_back(queue, newelem);
	in_queue[y][x] = 1;
	return (1);
}

int	add_neighbors_to_queue_careful(int x, int y, t_rlist **queue, t_img *img, int **in_queue)
{
	int	on_edge;
	int	total_added;

	total_added = 0;
	on_edge = is_on_edge(img, x + 1, y);
	if (on_edge != 2 && !in_queue[y][x + 1])
		total_added += add_to_queue(x + 1, y, queue, in_queue);
	on_edge = is_on_edge(img, x - 1, y);
	if (on_edge != 2 && !in_queue[y][x - 1])
		total_added += add_to_queue(x - 1, y, queue, in_queue);
	on_edge = is_on_edge(img, x, y + 1);
	if (on_edge != 2 && !in_queue[y + 1][x])
		total_added += add_to_queue(x, y + 1, queue, in_queue);
	on_edge = is_on_edge(img, x, y - 1);
	if (on_edge != 2 && !in_queue[y - 1][x])
		total_added += add_to_queue(x, y - 1, queue, in_queue);
	return (total_added);
}

int	add_neighbors_to_queue_greedy(int x, int y, t_rlist **queue, int **in_queue)
{
	int	total_added;

	total_added = 0;
	if (!in_queue[y][x + 1])
		total_added += add_to_queue(x + 1, y, queue, in_queue);
	if (!in_queue[y][x - 1])
		total_added += add_to_queue(x - 1, y, queue, in_queue);
	if (!in_queue[y + 1][x])
		total_added += add_to_queue(x, y + 1, queue, in_queue);
	if (!in_queue[y - 1][x])
		total_added += add_to_queue(x, y - 1, queue, in_queue);
	return (total_added);
}

// Greedy <=> not checking for outside of bounds
// Returns 1 when all neighbors have equal time,
// 		0 when at least one has different time
int	explore_neighbors_greedy(int x, int y, t_img *img, int **times)
{
	int	time; // TODO : rajouter une variable int[5] pour stocker les times joliment

	explore_one_point(x + 1, y, img, times);
	explore_one_point(x - 1, y, img, times);
	explore_one_point(x, y + 1, img, times);
	explore_one_point(x, y - 1, img, times);
	time = times[y][x];
	if (time == times[y][x + 1]
		&& time == times[y][x - 1]
		&& time == times[y + 1][x]
		&& time == times[y - 1][x])
		return (1);
	else
		return (0);
}

// <times> table values :
// 		- (-2) not explored
// 		- (-1) explored, belonged to Mandelbrot set
// 		- (x >= 0) already explored, had escape time x
// <in_queue> table values :
// 		- (0) never was in queue
// 		- (1) in queue
// 		- (2) has been through queue, and exited
void	draw_mandelbrot3(t_img *img)
{
	int		**in_queue;
	int		**times;
	t_rlist	*queue;
	int		queue_size;
	int		nb_border_pixels;
	int		time;
	int		ind;

	printf("[Method 3] Drawing Mandelbrot set with threshold %f on img at %p, NW coords = %f'%f\n",
		img->threshold_sq, img->img_ptr, img->nw_coords.x, img->nw_coords.y); // LOG WITH PRINTF
	draw_black(img);
	nb_border_pixels = 0;
	in_queue = allocate_intarray_2dim(img->width, img->height);
	times = allocate_intarray_2dim(img->width, img->height);
	queue = create_queue(img->width, img->height, in_queue, times);
	queue_size = img->width * 2 + img->height * 2;

	ind = 0;
	while (queue_size > 0 && ind < 20)
	{
		ft_printf(LOGS, "Exploring point in queue, pixels coords = %d'%d (queue size : %d)\n", queue->x, queue->y, queue_size);
		nb_border_pixels++;
		time = explore_one_point(queue->x, queue->y, img, times);
		if (is_on_edge(img, queue->x, queue->y))
			queue_size += add_neighbors_to_queue_careful(queue->x, queue->y, &queue, img, in_queue);
		else
		{
			if (explore_neighbors_greedy(queue->x, queue->y, img, times) == 0)
				queue_size += add_neighbors_to_queue_greedy(queue->x, queue->y, &queue, in_queue);
			 // prune search if : INSIDE a color band (the mandelbrot set being a color band)
		}
		if (queue_size > 1)
		{
			in_queue[queue->y][queue->x] = 2;
			queue = queue->next;
			ft_lstdelone(queue->prev);
		}
		queue_size--;
		ind++;
	}

	ft_printf(LOGS, "\tpixels computed for borders : %d\n", nb_border_pixels);
}
