/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:34:50 by alex              #+#    #+#             */
/*   Updated: 2025/03/20 20:42:18 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

void		ft_render(t_data *data);
void		ft_render_init_camera(t_data *data);
int			ft_render_sample_one_pixel(t_data *data, int pixel_x, int pixel_y);
// Ray color
t_vec		ft_draw_one_ray(t_data *data, t_ray ray);
int			ft_check_ray_shadows(t_data *data, t_ray ray, t_interval *hit_interval);
int			ft_intersect_ray_object(t_ray ray, t_object *object, t_interval *hit_interval, t_intersection *intersect);
t_vec		ft_color_ray_at(t_data *data, t_ray ray, t_intersection *intersect);
// Ray intersections
int			ft_intersect_ray_side(t_ray ray, t_side *side, t_interval *hit_interval, t_intersection *intersect);
int			ft_intersect_ray_cylinder(t_ray ray, t_object *cylinder, t_interval *hit_interval, t_intersection *intersect);
void		ft_intersect_ray_cylinder_body(t_ray ray, t_object *cylinder, t_interval *hit_interval, t_intersection *intersects);
int			ft_intersect_ray_sphere(t_ray ray, t_object *sphere, t_interval *hit_interval, t_intersection *intersect);
int			ft_intersect_ray_plane(t_ray ray, t_object *plane, t_interval *hit_interval, t_intersection *intersect);
int			ft_intersect_ray_cone(t_ray ray, t_object *cone, t_interval *hit_interval, t_intersection *intersect);
void		ft_intersect_ray_cone_body(t_ray ray, t_object *cone, t_interval *hit_interval, t_intersection *intersects);

// Shared between cylinder and cone logic
t_vec		ft_intersect_cyco_a(double dot_vd, t_vec v, t_vec d);
t_vec		ft_intersect_cyco_b(t_cycoeq *eq, double dot_vd, double dot_vs_vp);
t_vec		ft_intersect_cyco_c(t_cycoeq *eq, double dot_vs_vp);
int			ft_find_tmin_in_intersects(t_intersection *intersects, int nb_intersects);


// Texture mapping
void		ft_retrieve_texture(t_intersection *intersect, t_object *object, double ratio_vert, double ratio_hori);
t_vec		ft_derive_perturbation_normal(t_img_ppm *ppm, int x, int y);

// Logs
void		ft_render_log_camera(t_data *data);
void		ft_render_log_ray(t_ray ray, t_vec viewport_point, int pixel_x, int pixel_y);
void		ft_log_intersection(t_object *object, t_intersection *intersect);

t_vec		ft_color_from_lights(t_data *data, t_ray ray, t_intersection *intersect, int only_specular);
t_vec		ft_color_from_ambient(t_data *data);


#endif