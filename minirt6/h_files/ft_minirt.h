/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:14:48 by administyra       #+#    #+#             */
/*   Updated: 2025/03/10 18:58:42 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FRACTOL_H
# define FT_FRACTOL_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <float.h>
# include <math.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <time.h>
# include "../mlx/mlx.h"
# include "../libft/h_files/ft_get_next_line.h"
# include "../libft/h_files/libft.h"

# define LOGS 1
# define WIN_WIDTH 400
# define WIN_HEIGHT 225
# define NPIXELS WIN_HEIGHT * WIN_WIDTH

# define PI 3.1415926535
# define INFINITE_INT __INT_MAX__
# define INFINITE_DOUBLE __DBL_MAX__
# define MAX_RECURR_LVL 10

# define SPHERE 1
# define PLANE 2
# define CYLINDER 3

# define LAMBERTIAN 1
# define METAL 2

extern int	g_log;
extern int	g_count;

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

// A scene object, with generic attributes whose interpretation depends on type
// <type> =
// 		- SPHERE : center given by <start>, radius given by <dist>
// 		- PLANE : point given by <start>, normal given by <normal>
// 		- CYLINDER : spine defined by <start> and <normal>, size given by <height> and <dist>
// <material> =
// 		- LAMBERTIAN : diffuses light, but absorbs some as defined by <albedo>
// 		- METAL : reflects light, but absorbs some as defined by <albedo>
// <albedo> = what light is not absorbed by the object -> the object's color
// <sides> = array of objects of type PLANE, boundibg this object
// 			(eg. a cylinder has 2 sides, a hexahedre has 6 sides)
// <next> = next object in the linked list of scene objects
typedef struct s_object
{
	int				type;
	int				material;
	t_vec			start;
	t_vec			normal;
	double			dist;
	double			height;
	t_vec			albedo;
	int				unique_object_set;
	struct s_object	sides[6];

	struct s_object	*next;
}				t_object;

// Holds informations describing an intersection between the ray and an object
// 		- <object> the intersected object
// 		- <normal> the normal of the intersected surface at the crosspoint
// 		- <t> the scalar at which the ray intersects
// 		- <crosspoint> the coordinates of the intersection point
typedef struct s_intersection
{
	struct s_object	*object;
	t_vec			normal;
	double			t;
	t_vec			crosspoint;
}				t_intersection;

// Informations about the scene's camera :
// 		- its position and orientation vectors (direction + up/down)
// 		- its field of view angle (usually 90 degrees)
// 		- its viewport's coordinates and dimensions
typedef struct s_camera
{
	int		unique_object_set;

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
	struct s_data		*data;
}				t_img;

// Conveyer of data for MLX window instances
typedef struct s_data
{
	void		*mlx_ptr;
	void		*win_ptr;
	t_img		*img;

	double		win_ratio;
	int			nb_samples_per_pixel;
	int			recurr_lvl;

	t_cam		camera;
	t_object	*objects;
	t_object	alightning;
	t_object	lamp;
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
int			ft_parse_scene(char *scene_file, t_data *data);
void		ft_log_scene(t_data *data);
void		ft_calc_rt(t_data *data);
// Ray color
t_vec		ft_draw_one_ray(t_data *data, t_ray ray);
int			ft_calc_sample_one_pixel(t_data *data, int pixel_x, int pixel_y);
int			ft_intersect_ray_object(t_ray ray, t_object *object, t_interval *hit_interval, t_intersection *intersect);
t_vec		ft_color_ray_at(t_data *data, t_ray ray, t_intersection *intersect);
// Logs
void		ft_calc_log_camera(t_data *data);
void		ft_calc_log_ray(t_ray ray, t_vec viewport_point, int pixel_x, int pixel_y);
// Utils
double		ft_fmin(double a, double b);
double		ft_fmax(double a, double b);
double		ft_degrees_to_radians(double d);
double		ft_radians_to_degrees(double r);
double		ft_rand_double(void);
double		ft_rand_double_within(double min, double max);
double		ft_fabs(double a);
double		ft_atof(char *s);
// Linked list of objects
t_object	*ft_object_new();
void		ft_object_push_back(t_object **start, t_object *current);

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
int			ft_vec_from_str(t_vec *vec_dest, char *s);
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

= AF COURT TERME
- adapter le code actuel a la facon dont le parsing stocke les donnees
- observer et tester la version de Aleksei
- comprendre la couleur bizarre de la petite sphere
- implementer les dernieres features de "RT in 1 WE", pour pouvoir passer aux features du suivant


= CHOSES BIZARRES / QUESTIONS NON RESOLUES
- spheres deformees, d'autant plus quand place dans les coins du viewport
- pourquoi la diffusion non-uniforme a-t-elle un effet ? j'ai l'impression que les probas restent identiques
- pourquoi dans ma scene de test, avec une absorption faible, le haut de la petite sphere devient jaune ?

= A REPLIQUER DANS LA VERSION COMMUNE
- parsing :
	\ accepter les lignes vides
	\ refuser les entrees qui ne donnent pas les objets obligatoires (L, C, A)
	\ normalisation des vecteurs de directions donnes par le fichier
*/
