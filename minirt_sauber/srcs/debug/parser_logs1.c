/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logs1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:35:28 by alex              #+#    #+#             */
/*   Updated: 2025/03/19 18:28:01 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Logs the scene elements (mandatory and not mandatory)
void	ft_log_scene(t_data *data)
{
	printf("==========\n");
	printf("--- 1. Camera angle = %f, pos + dvec :\n", data->camera.fov_angle);
	ft_vec_display(data->camera.pos, 1);
	ft_vec_display(data->camera.dvec, 1);
	printf("--- 2. Ambient lightning at intensity %f, colored :\n",
		data->alightning.dist);
	ft_vec_display(data->alightning.albedo, 1);
	printf("--- 3. Light sources :\n");
	ft_log_objects(data->lights);
	printf("--- 4. Objects beginning at %p :\n", data->objects);
	ft_log_objects(data->objects);
	printf("==========\n");
}

// Logs a specific texture (a <t_img_ppm> structure)
void	ft_log_ppm(t_img_ppm *img)
{
	printf("PPM Image at %p\n", img);
	printf("Albedo : width %d and height %d\n",
		img->width_albedo, img->height_albedo);
	printf("Color of first and last pixel :\n");
	ft_vec_display(img->values_albedo[0][0], 1);
	ft_vec_display(
		img->values_albedo[img->height_albedo - 1][img->width_albedo - 1], 1);
	printf("Bump : width %d and height %d\n",
		img->width_bump, img->height_bump);
	printf("Height of first and last pixel : %f, %f\n",
		img->values_bump[0][0],
		img->values_bump[img->height_bump - 1][img->width_bump - 1]);
}

// Goes through the texture array to log each texture
void	ft_log_textures(t_img_ppm **textures, int nb)
{
	int	ind;

	ind = 0;
	while (ind < nb)
	{
		ft_log_ppm(textures[ind]);
		if (ind + 1 < nb)
			printf("-----------\n");
		ind++;
	}
	printf("==========\n");
}
