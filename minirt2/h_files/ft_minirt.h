/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minirt.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:14:48 by administyra       #+#    #+#             */
/*   Updated: 2025/02/01 18:01:05 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FRACTOL_H
# define FT_FRACTOL_H

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <limits.h>
# include <mlx.h>
# include <math.h>
# include <X11/keysym.h>
# include <X11/X.h>
# include <time.h>

// MACROS FOR VALUES INDEPENDENT FROM USER
# define LOGS 1
# define WIN_WIDTH 8
# define WIN_HEIGHT 4
# define NPIXELS WIN_HEIGHT * WIN_WIDTH
# define PI 3.1415926535

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}				t_vec;

typedef struct s_ray
{
	t_vec	orig;
	t_vec	dir;
}				t_ray;

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
	struct s_mlxdata	*data;
}				t_img;

// Conveyer of data for MLX window instances
typedef struct s_mlxdata
{
	void	*mlx_ptr;
	void	*win_ptr;
	double	win_ratio;
	t_img	*img;
}				t_mlxdata;

// === USE OF MLX

// Management of MLX images with intermediary <t_img>
int		ft_new_image(t_mlxdata *data, t_img *img, int width, int height);
void	ft_display_changed_image(t_mlxdata *data, t_img *img);
void	ft_change_pixel(t_img *img, int x, int y, int color);
// Creating colors
int		ft_assemble_color(int a, int r, int g, int b);
// Management of MLX events
int		ft_handle_destroy(t_mlxdata *data);
int		ft_handle_keyrelease(int keysym, t_mlxdata *data);
int		ft_handle_mouse(int button, int x, int y, t_mlxdata *data);

// === MINIRT CORE

// Core
void	ft_calc_rt(t_mlxdata *data);
// Utils
int		ft_printf(int mode, const char *s, ...);
// Vector operations
t_vec	ft_vec_copy(t_vec v);
t_vec	ft_vec_init(double x, double y, double z);
t_vec	ft_vec_add(t_vec v1, t_vec v2);
t_vec	ft_vec_neg(t_vec v);
t_vec	ft_vec_substr(t_vec v1, t_vec v2);
t_vec	ft_vec_scale(t_vec v, double a);
double	ft_vec_norm_sq(t_vec v);
double	ft_vec_norm(t_vec v);
t_vec	ft_vec_setnorm(t_vec v, double new_norm);
double	ft_vec_sproduct(t_vec v1, t_vec v2);
t_vec	ft_vec_vproduct(t_vec v1, t_vec v2);
void	ft_vec_display(t_vec v, int newline);
// Rays operations
t_vec	ft_ray_at(t_ray ray, double p);
t_ray	ft_ray_init(t_vec orig, t_vec dir);
void	ft_ray_display(t_ray ray, int newline);


#endif
