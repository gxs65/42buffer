/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:13:34 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 20:55:29 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Minimum of 2 ints
int	ft_min(int a, int b)
{
	if (a <= b)
		return (a);
	else
		return (b);
}

// Maximum of 2 ints
int	ft_max(int a, int b)
{
	if (a >= b)
		return (a);
	else
		return (b);
}

// Creates the <slopes> vectors used by <ft_derive_perturbation_normal>,
// by retrieving and substracting the heights at this point and at next points
// in x and y directions, but with 2 corrections on z coordinate :
// 		- as a slope is `(f(x + h) - f(x)) / h`, the difference in height (z)
// 		  should be divided by h the difference in x and y,
// 			and here this difference is (1 / ppm_dim) since we advanced
// 			by exactly 1 pixel in the x and y directions
// 				so we divide by (1 / ppm_dim) <=> multiply by ppm_dim
// 		- this produces very sharp textures, attenuated by a fixed factor
// 		  defined by macro ATTENUATION FACTOR (should be aroune 1/100)
void	ft_texture_bump_find_slopes(t_img_ppm *ppm, int x, int y, t_vec *slopes)
{
	int		x_next;
	int		y_next;
	double	height_x_next;
	double	height_y_next;
	double	height_here;

	x_next = ft_min(x + 1, ppm->width_bump - 1);
	y_next = ft_min(y + 1, ppm->height_bump - 1);
	height_here = ppm->values_bump[y][x];
	height_x_next = ppm->values_bump[y][x_next];
	height_y_next = ppm->values_bump[y_next][x];
	slopes[0] = ft_vec_init(1, 0, (height_x_next - height_here)
			* (double)(ppm->width_bump) * BUMP_ATTENUATION);
	slopes[1] = ft_vec_init(0, 1, (height_y_next - height_here)
			* (double)(ppm->height_bump) * BUMP_ATTENUATION);
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURES - BUMP] at x %d y %d, x_next %d y_next %d\n",
			x, y, x_next, y_next);
		printf("[TEXTURES - BUMP] heights : here %f, x+1 %f, y+1 %f\n",
			height_here, height_x_next, height_y_next);
	}
}

// Computes the "perturbation normal" (normal at point on the texture in 2D) :
// the values stored in <ppm.values_bump> are not directly normal vectors,
// but merely the height of each point, from which we compute a normal
// Ideally the normal should be calculated using the partial derivatives
// of the height with respect to x and y,
// but with discrete points we do a close method :
// 		store in <slopes> the vectors from the current point to the next points
// 		in the x and y directions -> they describe a change in height (z),
// 			then compute normal as vector product of these two <slopes>
t_vec	ft_derive_perturbation_normal(t_img_ppm *ppm, int x, int y)
{	
	t_vec	slopes[2];
	t_vec	res;

	ft_texture_bump_find_slopes(ppm, x, y, slopes);
	res = ft_vec_setnorm(ft_vec_vproduct(slopes[0], slopes[1]), 1);
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURES - BUMP] slope on x, slope on y, bump normal :\n");
		ft_vec_display(slopes[0], 1);
		ft_vec_display(slopes[1], 1);
		ft_vec_display(res, 1);
	}
	return (res);
}
