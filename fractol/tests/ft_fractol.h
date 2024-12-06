#ifndef FT_FRACTOL_H
# define FT_FRACTOL_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <mlx.h>
# include <math.h>

# define LOGS 1
# define WIN_WIDTH 1000
# define WIN_HEIGHT 1000
# define MAX_ITER 100

// Representation of a point on the plane, can be interpreted
// 		- classically : eg. compute distance between 1 points (<dist> function)
// 		- in the complex plane : <x> real part, <y> imaginary part
typedef struct	s_point
{
	double	x;
	double	y;
}				t_point;

// Conveyer of data for MLX images dedicated to fractal drawings
// 		- addr : a pointer to the address of the first pixel in the image
// 		- bpp/opp : in how much bits/bytes a pixel is encoded
// 		- width/height : dimensions in pixels
// 		- line_length : space taken in memory by the data of pixels of a line
// 						= width * opp	(-> unit is bytes)
// 		- endian : ? for now
// 		- nw_coords : where the upperleft corner of the image should be
// 					  on the COMPLEX plane
//		- scale : how many units in the complex plane the image width covers
// 				  (eg. if set to 2 with width = 700pix, a segment of length 1
// 				   on the plane will be represented by 350 pixels)
typedef struct	s_img {
	void	*img_ptr;
	char	*addr;
	int		height;
	int		width;
	int		bpp;
	int		opp;
	int		line_length;
	int		endian;
	t_point	nw_coords;
	double	scale;
}				t_img;

// Utils
int		ft_printf(int mode, const char *s, ...);
void	log_imgdata(t_img *img);
int		assemble_color(int a, int r, int g, int b);

// MLX drawings
void	new_image(void *mlx, t_img *img, int width, int height);
void	change_pixel(t_img *img, int x, int y, int color);
void	draw_circle(t_img *img, t_point *center, double radius, int color);
void	draw_julia(t_img *img, t_point *c, double threshold);

// Geometry
double	dist(t_point *pa, t_point *pb);

// Complex operations
void	add_complex(t_point *a, t_point *b, t_point *res);
void	multiply_complex(t_point *a, t_point *b, t_point *res);
void	apply_complex_qmap_inplace(t_point *p, t_point *c);
int		time_to_diverge(t_point *p, t_point *c, double threshold, int max_iter);

#endif


/*
TODO

- fonctions de libft a inclure :
	\ atoi
- free les pointeurs renvoyes par fonctions MLX
- proteger les appels aux fonctions MLX (if (!pointer))
*/