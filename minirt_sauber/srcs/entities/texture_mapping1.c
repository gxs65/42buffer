/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_mapping1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 14:47:45 by abedin            #+#    #+#             */
/*   Updated: 2025/03/19 20:55:09 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Manages checkered textures :
// 		the intersection normal is not modified,
// 		and the color can only by white or black
// 			according to the parity of x and y components
void	ft_retrieve_checkered_texture(t_intersection *intersect,
	t_object *object, double ratio_vert, double ratio_hori)
{
	int	nboxes_x;
	int	nboxes_y;

	if (object->type == SPHERE)
	{
		nboxes_x = 20;
		nboxes_y = 12;
	}
	else
	{
		nboxes_x = 8;
		nboxes_y = 8;
	}
	if (((int)(ratio_vert * nboxes_y)) % 2
		== ((int)(ratio_hori * nboxes_x) % 2))
		intersect->albedo = ft_vec_init(1, 1, 1);
	else
		intersect->albedo = ft_vec_init(0, 0, 0);
}

// Determines how to modify the normal at intersection point (bump mapping)
// 		a) <ft_derive_perturbation_normal> uses the texture <ppm>
// 		   to calculate the normal we should have ON THE XY PLANE,
// 		   which means abstracted of the object's form and position in our scene
// 		b) this function receives the <perturbation normal> and reorients it
// 		   according to object's position <=> applies a transformation to it :
// 		  		the normal is transformed to the new coordinate system
// 				defined by the intersection's normal, tangent and bitangent
void	ft_retrieve_texture_bump(t_intersection *intersect, t_img_ppm *ppm,
	int x, int y)
{
	t_vec		perturbation_normal;
	t_vec		rotation_vecs[3];

	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURE - BUMP] pix : x %d, y %d\n", x, y);
		printf("[TEXTURE - BUMP] normal before bump mapping : ");
		ft_vec_display(intersect->normal, 1);
	}
	perturbation_normal = ft_derive_perturbation_normal(ppm, x, y);
	rotation_vecs[0] = intersect->tangent;
	rotation_vecs[1] = intersect->bitangent;
	rotation_vecs[2] = intersect->normal;
	intersect->normal = ft_vec_setnorm(ft_vec_transform_by_matrix(
				perturbation_normal, rotation_vecs), 1);
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURES - BUMP] normal after bump mapping : ");
		ft_vec_display(intersect->normal, 1);
	}
}

// For a given intersection point on an object of any type,
// uses the <ratio_vert> and <ratio_hori> computed by the object's logic
// to map the intersection point (3D) to a point on a surface (2D),
// 		then uses the color and height at that point on the object's texture
//		to modify the intersection point's color and normal
// 1) Modifying color : easy, just retrieve the color of the texture
// 						at the x'y point and set <intersect->albedo> to this
// 2) Modifying normal (= "bump mapping") : much harder
// 			- the goal is to produce an illusion of material roughness
// 			  by influencing how light is computed at that point,
// 				which depends a lot on the normal
// 			- but the object's texture does not directly give the normal
// 			  at a given point, but rather the height of this point
// 				so <retrieve_texture_bump> will have to do more calculations
void	ft_retrieve_texture(t_intersection *intersect, t_object *object,
	double ratio_vert, double ratio_hori)
{
	t_img_ppm	*ppm;
	int			x;
	int			y;

	ratio_hori = ft_fmin(ratio_hori, 1 - 0.001);
	ratio_vert = ft_fmin(ratio_vert, 1 - 0.001);
	if (object->fd_texture == -2)
	{
		ft_retrieve_checkered_texture(intersect, object,
			ratio_vert, ratio_hori);
		return ;
	}
	ppm = object->ppm_texture;
	x = (int)(ratio_hori * ppm->width_albedo);
	y = (int)(ratio_vert * ppm->height_albedo);
	intersect->albedo = ppm->values_albedo[y][x];
	if (g_log & B_TEXTURES)
	{
		printf("[TEXTURES - ALBEDO] ratio to pix : x %f -> %d, y %f -> %d\n",
			ratio_hori, x, ratio_vert, y);
	}
	x = (int)(ratio_hori * ppm->width_bump);
	y = (int)(ratio_vert * ppm->height_bump);
	ft_retrieve_texture_bump(intersect, ppm, x, y);
}
