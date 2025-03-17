/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:34:50 by alex              #+#    #+#             */
/*   Updated: 2025/03/17 17:22:20 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

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

typedef struct s_side
{
	t_vec	start;
	t_vec	normal;
	double	dist;
}				t_side;

typedef struct s_img_ppm
{
	t_vec	**values;
	int		width;
	int		height;
}				t_img_ppm;

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
// <orient_left>, <orient_up> = orientation definition for objects
// 		with no obvious orientation, eg. spheres (necessary for applying textures)
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
	struct s_side	sides[6];
	t_vec			orient_up;
	t_vec			orient_left;
	t_vec			orient_front;
	int				fd_texture;
	t_img_ppm		*ppm_texture;

	struct s_object	*next;
}				t_object;
// Linked list of objects
t_object	*ft_object_new();
void		ft_object_push_back(t_object **start, t_object *current);

// Holds informations describing an intersection between the ray and an object
// 		- <object> the intersected object
// 		- <normal> the normal of the intersected surface at the crosspoint
// 		- <t> the scalar at which the ray intersects
// 		- <crosspoint> the coordinates of the intersection point
// 		- <albedo> the color of the object at the intersection point
typedef struct s_intersection
{
	struct s_object	*object;
	t_vec			normal;
	double			t;
	t_vec			crosspoint;
	t_vec			albedo;
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
	t_object	*lights;
	t_object	alightning;
	t_img_ppm	**textures;
}				t_data;

#endif