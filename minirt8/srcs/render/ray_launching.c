/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_launching.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: administyrateur <administyrateur@studen    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:37:34 by abedin            #+#    #+#             */
/*   Updated: 2025/03/15 11:24:03 by administyra      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ft_calc_init_camera(t_data *data)
{
	t_cam	*camera;

	camera = &(data->camera);
	camera->dvec = ft_vec_setnorm(camera->dvec, 1.0);
	camera->hvec = ft_vec_setnorm(camera->hvec, 1.0);
	camera->vvec = ft_vec_setnorm(ft_vec_vproduct(camera->dvec, camera->hvec), 1.0);

	camera->vport_center = ft_vec_add(camera->pos, camera->dvec);
	camera->vport_width = 2 * tan(ft_degrees_to_radians(camera->fov_angle) / 2);
	camera->vport_pix_width = camera->vport_width / (double)(data->img->width);
	camera->vport_height = camera->vport_width * data->win_ratio;
	camera->vport_pix_height = camera->vport_height / (double)(data->img->height);

	camera->vport_ul = 	ft_vec_add(camera->vport_center,
						ft_vec_add(	ft_vec_scale(camera->vvec, -1 * camera->vport_height / 2),
									ft_vec_scale(camera->hvec, -1 * camera->vport_width / 2)));
	camera->vport_ur = 	ft_vec_add(camera->vport_center,
						ft_vec_add(	ft_vec_scale(camera->vvec, -1 * camera->vport_height / 2),
									ft_vec_scale(camera->hvec, 1 * camera->vport_width / 2)));
	camera->vport_dl = 	ft_vec_add(camera->vport_center,
						ft_vec_add(	ft_vec_scale(camera->vvec, 1 * camera->vport_height / 2),
									ft_vec_scale(camera->hvec, -1 * camera->vport_width / 2)));
	camera->vport_dr = 	ft_vec_add(camera->vport_center,
						ft_vec_add(	ft_vec_scale(camera->vvec, 1 * camera->vport_height / 2),
									ft_vec_scale(camera->hvec, 1 * camera->vport_width / 2)));
}

int	ft_calc_sample_one_pixel(t_data *data, int pixel_x, int pixel_y)
{
	t_vec	viewport_point;
	t_ray	ray;
	int		ind;
	t_vec	color_components;
	t_cam	*camera;
	int		color;

	camera = &(data->camera);
	color_components = ft_vec_init(0, 0, 0);
	ind = 0;
	while (ind < data->nb_samples_per_pixel)
	{
		if (data->nb_samples_per_pixel > 1)
		{
			viewport_point = ft_vec_add(camera->vport_ul, ft_vec_add(
								ft_vec_add(	ft_vec_scale(camera->hvec, camera->vport_pix_width * ((double)pixel_x + 0.5)),
											ft_vec_scale(camera->vvec, camera->vport_pix_height * ((double)pixel_y + 0.5))),
								ft_vec_add(	ft_vec_scale(camera->hvec, camera->vport_pix_width * (ft_rand_double() - 0.5)),
											ft_vec_scale(camera->vvec, camera->vport_pix_height * (ft_rand_double() - 0.5)))));
		}
		else
		{
			viewport_point = ft_vec_add(camera->vport_ul,
				ft_vec_add(	ft_vec_scale(camera->hvec, camera->vport_pix_width * ((double)pixel_x + 0.5)),
							ft_vec_scale(camera->vvec, camera->vport_pix_height * ((double)pixel_y + 0.5))));
		}
		ray = ft_ray_init_by_point(camera->pos, viewport_point);
		if (g_log)
			printf("\n\nPoint %d'%d : launching ray %d\n", pixel_x, pixel_y, ind);
		color_components = ft_vec_add(color_components, ft_draw_one_ray(data, ray));
		ind++;
	}
	if (g_log)
	{
		printf("== Point %d'%d\n", pixel_x, pixel_y);
		printf("Final color components (sum of all samples, then mean of samples) :\n");
		ft_vec_display(color_components, 1);
		ft_vec_display(ft_vec_scale(color_components, (double)1 / (double)data->nb_samples_per_pixel), 1);
	}
	color = ft_assemble_color(ft_vec_scale(color_components, (double)1 / (double)data->nb_samples_per_pixel));
	if (g_log)
		printf("Color integer generated : %d\n", color);
	return (color);
}

// For each pixel on the viewport, determines the ray camera-pixel,
// 		and calls the function that will calculate the pixel's color
void	ft_calc_draw_rays(t_data *data)
{
	int		pixel_x;
	int		pixel_y;
	int		color;

	pixel_y = 0;
	while (pixel_y < data->img->height)
	{
		if (pixel_y == 99)
			g_log = 0;
		else
			g_log = 0;
		pixel_x = 0;
		while (pixel_x < data->img->width)
		{
			color = ft_calc_sample_one_pixel(data, pixel_x, pixel_y);
			ft_change_pixel(data->img, pixel_x, pixel_y, color);
			pixel_x++;
		}
		if (pixel_y % 10 == 0)
			printf("Line %d / %d complete\n", pixel_y, data->img->height);
		pixel_y++;
	}
}

void	ft_calc_rt(t_data *data)
{
	ft_calc_init_camera(data);
	ft_calc_log_camera(data);
	ft_calc_draw_rays(data);
}