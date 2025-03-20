/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:00:59 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 17:03:27 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Computes the edge point of the camera's viewport by taking the vport center
// and going NorthWest/NorthEast/SouthWest/SouthEast
void	ft_render_init_camera2(t_cam *camera)
{
	camera->vport_ul = ft_vec_add(camera->vport_center,
			ft_vec_add(ft_vec_scale(
					camera->vvec, -1 * camera->vport_height / 2),
				ft_vec_scale(camera->hvec, -1 * camera->vport_width / 2)));
	camera->vport_ur = ft_vec_add(camera->vport_center,
			ft_vec_add(ft_vec_scale(
					camera->vvec, -1 * camera->vport_height / 2),
				ft_vec_scale(camera->hvec, 1 * camera->vport_width / 2)));
	camera->vport_dl = ft_vec_add(camera->vport_center,
			ft_vec_add(ft_vec_scale(
					camera->vvec, 1 * camera->vport_height / 2),
				ft_vec_scale(camera->hvec, -1 * camera->vport_width / 2)));
	camera->vport_dr = ft_vec_add(camera->vport_center,
			ft_vec_add(ft_vec_scale(
					camera->vvec, 1 * camera->vport_height / 2),
				ft_vec_scale(camera->hvec, 1 * camera->vport_width / 2)));
}

// Computes the camera's orientation vectors
// (dvec and hvec given in scene description, vvec orthogonal to them both)
// and viewport parameters
// The viewport is the virtual rectangle delimiting where we launch rays :
// 		- its center is directly along the camera's direction vector <dvec>,
// 		- its width is then defined (through trigonometry)
// 		  by the angle of field of viex <camera.fov_angle>
// 		- its height is then derived from the window's aspect ratio
// 		- it has scalars <vport_pix_width/height> defining how much to go
// 		  along vvec and hvec vectors to advance by 1 pixel on the image
void	ft_render_init_camera(t_data *data)
{
	t_cam	*camera;

	camera = &(data->camera);
	camera->dvec = ft_vec_setnorm(camera->dvec, 1.0);
	camera->hvec = ft_vec_setnorm(camera->hvec, 1.0);
	camera->vvec = ft_vec_setnorm(ft_vec_vproduct(
				camera->dvec, camera->hvec), 1.0);
	camera->vport_center = ft_vec_add(camera->pos, camera->dvec);
	camera->vport_width = 2 * tan(ft_degrees_to_radians(camera->fov_angle) / 2);
	camera->vport_pix_width = camera->vport_width / (double)(data->img->width);
	camera->vport_height = camera->vport_width * data->win_ratio;
	camera->vport_pix_height = camera->vport_height
		/ (double)(data->img->height);
	ft_render_init_camera2(camera);
}
