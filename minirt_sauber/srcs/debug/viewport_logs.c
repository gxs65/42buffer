/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   viewport_logs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:34:03 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 18:32:12 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Logs all viewport / camera parameters, after they were computed
void	ft_render_log_camera(t_data *data)
{
	t_cam	*camera;

	camera = &(data->camera);
	printf("Camera def = pos, dvec, hvec, vvec (+ angle : %f) :\n",
		camera->fov_angle);
	ft_vec_display(camera->pos, 1);
	ft_vec_display(camera->dvec, 1);
	ft_vec_display(camera->hvec, 1);
	ft_vec_display(camera->vvec, 1);
	printf("Viewport = width %f (pix : %f), height %f (pix : %f), center :\n",
		camera->vport_width, camera->vport_pix_width,
		camera->vport_height, camera->vport_pix_height);
	ft_vec_display(camera->vport_center, 1);
	printf("Viewport edge points = UpLeft, UpRight, DownLeft, DownRight :\n");
	ft_vec_display(camera->vport_ul, 1);
	ft_vec_display(camera->vport_ur, 1);
	ft_vec_display(camera->vport_dl, 1);
	ft_vec_display(camera->vport_dr, 1);
}

// Logs a single ray
void	ft_render_log_ray(t_ray ray, t_vec viewport_point,
	int pixel_x, int pixel_y)
{
	printf("Pixel %d'%d position on viewport : ", pixel_x, pixel_y);
	ft_vec_display(viewport_point, 0);
	printf(" -> ray : ");
	ft_ray_display(ray, 1);
}
