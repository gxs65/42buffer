/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:34:50 by alex              #+#    #+#             */
/*   Updated: 2025/03/14 19:00:36 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

void		ft_calc_rt(t_data *data);
// Ray color
t_vec		ft_draw_one_ray(t_data *data, t_ray ray);
int			ft_check_ray_shadows(t_data *data, t_ray ray, t_interval *hit_interval);
int			ft_calc_sample_one_pixel(t_data *data, int pixel_x, int pixel_y);
int			ft_intersect_ray_object(t_ray ray, t_object *object, t_interval *hit_interval, t_intersection *intersect);
t_vec		ft_color_ray_at(t_data *data, t_ray ray, t_intersection *intersect);
// Logs
void		ft_calc_log_camera(t_data *data);
void		ft_calc_log_ray(t_ray ray, t_vec viewport_point, int pixel_x, int pixel_y);
// Ray intersections
int			ft_intersect_ray_side(t_ray ray, t_side *side, t_interval *hit_interval, t_intersection *intersect);
int			ft_intersect_ray_cylinder(t_ray ray, t_object *cylinder, t_interval *hit_interval, t_intersection *intersect);
int			ft_intersect_ray_sphere(t_ray ray, t_object *sphere, t_interval *hit_interval, t_intersection *intersect);
int			ft_intersect_ray_plane(t_ray ray, t_object *plane, t_interval *hit_interval, t_intersection *intersect);
int			ft_intersect_ray_cone(t_ray ray, t_object *cone, t_interval *hit_interval, t_intersection *intersect);

// Utils
double	ft_atof(char *s);

#endif