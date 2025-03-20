/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_launching.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:37:34 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 17:12:11 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/*
t_vec	ft_pixel_coords_to_vport(t_cam *camera, int pixel_x, int pixel_y,
	int randomize)
{
	t_vec	vport_point;

	if (randomize)
	{
		vport_point = ft_vec_add(camera->vport_ul, ft_vec_add(
			ft_vec_add(ft_vec_scale(camera->hvec,
					camera->vport_pix_width * ((double)pixel_x + 0.5)),
				ft_vec_scale(camera->vvec,
					camera->vport_pix_height * ((double)pixel_y + 0.5))),
			ft_vec_add(ft_vec_scale(camera->hvec,
					camera->vport_pix_width * (ft_rand_double() - 0.5)),
				ft_vec_scale(camera->vvec,
					camera->vport_pix_height * (ft_rand_double() - 0.5)))));
	}
	else
	{
		vport_point = ft_vec_add(camera->vport_ul,
			ft_vec_add(ft_vec_scale(camera->hvec,
					camera->vport_pix_width * ((double)pixel_x + 0.5)),
				ft_vec_scale(camera->vvec,
					camera->vport_pix_height * ((double)pixel_y + 0.5))));
	}
	return (vport_point);
}
*/

// Converts a pixel's coordinates on the image
// to the 3D coordinates in our scene of the corresponding point on the viewport
// 		with a slight randomization if <randomize is set to 1
// 		(within the limits of the area covered by the pixel on the viewport)
t_vec	ft_pixel_coords_to_vport(t_cam *camera, int pixel_x, int pixel_y,
	int randomize)
{
	t_vec	vport_point;
	t_vec	pixel_ul;

	pixel_ul = ft_vec_add(camera->vport_ul,
			ft_vec_add(ft_vec_scale(camera->hvec,
					camera->vport_pix_width * (double)pixel_x),
				ft_vec_scale(camera->vvec,
					camera->vport_pix_height * (double)pixel_y)));
	if (randomize)
	{
		vport_point = ft_vec_add(pixel_ul,
				ft_vec_add(ft_vec_scale(camera->hvec,
						camera->vport_pix_width * ft_rand_double()),
					ft_vec_scale(camera->vvec,
						camera->vport_pix_height * ft_rand_double())));
	}
	else
	{
		vport_point = ft_vec_add(pixel_ul,
				ft_vec_add(ft_vec_scale(camera->hvec,
						camera->vport_pix_width * 0.5),
					ft_vec_scale(camera->vvec,
						camera->vport_pix_height * 0.5)));
	}
	return (vport_point);
}

// Determines a pixel's color by sending <data.nb_sample_per_pixel> rays
// (with a slight randomization of direction if more than 1 ray is sent)
// and averaging the colors returned by <ft_draw_one_ray>
int	ft_render_sample_one_pixel(t_data *data, int pixel_x, int pixel_y)
{
	t_vec	vport_point;
	t_ray	ray;
	int		ind;
	t_vec	color;

	color = ft_vec_init(0, 0, 0);
	ind = 0;
	while (ind < data->nb_samples_per_pixel)
	{
		vport_point = ft_pixel_coords_to_vport(&data->camera, pixel_x, pixel_y,
				(data->nb_samples_per_pixel > 1));
		ray = ft_ray_init_by_point(data->camera.pos, vport_point);
		if (g_log)
			printf("\n\nPoint %d'%d : launching ray %d\n", pixel_x, pixel_y, ind);
		color = ft_vec_add(color, ft_draw_one_ray(data, ray));
		ind++;
	}
	color = ft_vec_scale(color, 1.0 / (double)data->nb_samples_per_pixel);
	if (g_log)
	{
		printf("== Point %d'%d c\n", pixel_x, pixel_y);
		printf("Final mean color vec :\n");
		ft_vec_display(color, 1);
	}
	return (ft_assemble_color(color));
}

// Goes through each pixel of the image,
// calls the raytracing function to launch ray for this pixel,
// retrieves the pixel color and updates it in the image
// + logs the progress of the image generation
void	ft_render_draw_rays(t_data *data)
{
	int		pixel_x;
	int		pixel_y;
	int		color;

	pixel_y = 0;
	while (pixel_y < data->img->height)
	{
		pixel_x = 0;
		while (pixel_x < data->img->width)
		{
			color = ft_render_sample_one_pixel(data, pixel_x, pixel_y);
			ft_change_pixel(data->img, pixel_x, pixel_y, color);
			pixel_x++;
		}
		if (pixel_y % 10 == 0)
			printf("Line %d / %d complete\n", pixel_y, data->img->height);
		pixel_y++;
	}
}

// Initializes the camera and viewport, then begins the raytracing process
void	ft_render(t_data *data)
{
	g_log = 0;
	ft_render_init_camera(data);
	ft_render_log_camera(data);
	ft_render_draw_rays(data);
}
