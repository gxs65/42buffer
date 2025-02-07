/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:14:48 by administyra       #+#    #+#             */
/*   Updated: 2025/02/06 21:21:49 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FRACTOL_H
# define FT_FRACTOL_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <float.h>
# include <mlx.h>
# include <math.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <time.h>

# define LOGS 1
# define WIN_WIDTH 400
# define WIN_HEIGHT 225
# define NPIXELS WIN_HEIGHT * WIN_WIDTH

# define PI 3.1415926535
# define INFINITE_INT __INT_MAX__
# define INFINITE_DOUBLE __DBL_MAX__

# define SPHERE 1
# define RHOMBOID 2
# define CYLINDER 3

# define LAMBERTIAN 1
# define METAL 2

typedef struct s_interval
{
	double	min;
	double	max;
	double	size;
}				t_interval;

// Coordinates in a 3-dimensional space, to represent both :
// 		- a point in the space
// 		- a vector (<=> how to move from 1 point to another)
typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}				t_vec;

// A ray, half-line starting at point <orig> and oriented by direction <dir>
// <=> defined by equation : point = origin + t . (direction)
typedef struct s_ray
{
	t_vec	orig;
	t_vec	dir;
}				t_ray;

// A scene object, with generic attributes whose interpretation depends on :
// type =
// 		- SPHERE : center given by <start>, radius given by <dist>
// material =
// 		- LAMBERTIAN : diffuses light, but absorbs some as defined by <albedo>
// 		- METAL : reflects light, but reflects some as defined by <albedo>
typedef struct s_object
{
	int		type;
	int		material;
	t_vec	start;
	double	dist;
	t_vec	albedo;
}				t_object;

// Informations about the scene's camera :
// 		- its position and orientation vectors (direction + up/down)
// 		- its field of view angle (usually 90 degrees)
// 		- its viewport's coordinates and dimensions
typedef struct s_camera
{
	t_vec	pos;
	t_vec	dvec;
	t_vec	hvec;
	t_vec	vvec;
	double	fov_angle;

	t_vec	vport_center;
	double	vport_width;
	double	vport_pix_width;
	double	vport_height;
	double	vport_pix_height;
	t_vec	vport_ul;
	t_vec	vport_ur;
	t_vec	vport_dl;
	t_vec	vport_dr;
}				t_cam;

// Conveyer of data for MLX images dedicated to fractal drawings
// 	MLX IMAGES
// 		- img_ptr : a pointer to the image not being currently displayed
// 		- img_caches : the pointers to both images (displayed and modifiable)
// 		- which_img : the index of the inage not being currently displayed
// 	PIXEL REPRESENTATION
// 		- addr : the pointers to the addresses
// 				 of the first pixel in each of the 2 MLX image
// 		- bpp/opp : in how much bits/bytes a pixel is encoded
// 		- width/height : dimensions in pixels
// 		- line_length : space taken in memory by the data of pixels of a line
// 						= width * opp	(-> unit is bytes)
// 		- endian : here, ordering of the 4 bytes of ARGB
// 				   in the 4-bytes integers describing colors to MLX functions
typedef struct s_img
{
	void				*img_ptr;
	void				*img_caches[2];
	int					which_img;
	char				*addr[2];
	int					height;
	int					width;
	int					bpp;
	int					opp;
	int					line_length;
	int					endian;
	struct s_data	*data;
}				t_img;

// Conveyer of data for MLX window instances
typedef struct s_data
{
	void		*mlx_ptr;
	void		*win_ptr;
	double		win_ratio;
	t_img		*img;
	t_object	*objects;
	int			nb_samples_per_pixel;
	int			recurr_lvl;
	t_cam		camera;
}				t_data;

// === USE OF MLX

// Management of MLX images with intermediary <t_img>
int			ft_new_image(t_data *data, t_img *img, int width, int height);
void		ft_display_changed_image(t_data *data, t_img *img);
void		ft_change_pixel(t_img *img, int x, int y, int color);
// Creating colors
int			ft_assemble_color(t_vec color_component);
// Management of MLX events
int			ft_handle_destroy(t_data *data);
int			ft_handle_keyrelease(int keysym, t_data *data);
int			ft_handle_mouse(int button, int x, int y, t_data *data);

// === MINIRT CORE

// Core
int			ft_load_objects(t_data *data);
void		ft_calc_rt(t_data *data);
// Utils
int			ft_printf(int mode, const char *s, ...);
// Ray color
t_vec		ft_draw_one_ray(t_data *data, t_ray ray);
double		ft_intersect_ray_object(t_ray ray, t_object object, t_interval *hit_interval);
t_vec		ft_color_ray_at(t_data *data, int ind_min, t_ray ray, double t_min);
// Logs
void		ft_calc_log_camera(t_cam *camera);
void		ft_calc_log_ray(t_ray ray, t_vec viewport_point, int pixel_x, int pixel_y);
// Utils
double		ft_fmin(double a, double b);
double		ft_fmax(double a, double b);
double		ft_degrees_to_radians(double d);
double		ft_radians_to_degrees(double r);
double		ft_rand_double(void);
double		ft_rand_double_within(double min, double max);
double		ft_fabs(double a);

// === OPERATIONS

// Vector operations
t_vec		ft_vec_copy(t_vec v);
t_vec		ft_vec_init(double x, double y, double z);
t_vec		ft_vec_add(t_vec v1, t_vec v2);
t_vec		ft_vec_neg(t_vec v);
t_vec		ft_vec_substr(t_vec v1, t_vec v2);
t_vec		ft_vec_scale(t_vec v, double a);
t_vec		ft_vec_scale_by_vec(t_vec v1, t_vec v2);
double		ft_vec_norm_sq(t_vec v);
double		ft_vec_norm(t_vec v);
t_vec		ft_vec_setnorm(t_vec v, double new_norm);
double		ft_vec_sproduct(t_vec v1, t_vec v2);
t_vec		ft_vec_vproduct(t_vec v1, t_vec v2);
void		ft_vec_display(t_vec v, int newline);
t_vec		ft_vec_init_random_within(double min, double max);
t_vec		ft_vec_init_random_unit(void);
int			ft_vec_is_infinitesimal(t_vec v);
t_vec		ft_vec_symmetric_by_normal(t_vec incident, t_vec normal);
// Rays operations
t_vec		ft_ray_at(t_ray ray, double p);
t_ray		ft_ray_init_by_point(t_vec orig, t_vec dir);
t_ray		ft_ray_init_by_dir(t_vec orig, t_vec dir);
void		ft_ray_display(t_ray ray, int newline);
// Standard mathematics
int			ft_ops_solve_quadratic(double a, double b, double c, double *solutions);
// Intervals
t_interval	ft_interval_init(double min, double max);
int			ft_interval_belongs(t_interval *interval, double d);


#endif


/*
= A FAIRE
- renouveler mot de passe gmail
- jeter les dernieres bouteilles de lait
- lessive

= COMMITMENTS
- pas plus de 3 manches de SWBF2 par jour
- coucher a 1h du matin maximum
- escalade 1 fois par semaine
- manger au RU 2 fois par semaine

= CHOSES BIZARRES / QUESTIONS NON RESOLUES
- spheres deformees, d'autant plus quand place dans les coins du viewport
- pourquoi la diffusion non-uniforme a-t-elle un effet ? j'ai l'impression que les probas restent identiques
- pourquoi dans ma scene de test, avec une absorption faible, le haut de la petite sphere devient jaune ?

*/
