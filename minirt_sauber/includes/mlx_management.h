/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_management.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:46:21 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 21:08:30 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_MANAGEMENT_H
# define MLX_MANAGEMENT_H

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
int			ft_handle_loop(t_data *data);


# endif