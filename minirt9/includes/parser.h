/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:34:50 by alex              #+#    #+#             */
/*   Updated: 2025/03/17 17:01:52 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

int			ft_parse_scene(char *scene_file, t_data *data);
void		ft_log_scene(t_data *data);

int			ft_register_sphere(char **words, int nb_words, t_data *data);
int			ft_register_plane(char **words, int nb_words, t_data *data);
void		ft_det_cylinder_sides(t_object *cylinder);
int			ft_register_cylinder(char **words, int nb_words, t_data *data);
int			ft_register_cone(char **words, int nb_words, t_data *data);

int			ft_register_alightning(char **words, int nb_words, t_data *data);
int			ft_register_lamp(char **words, int nb_words, t_data *data);
int			ft_register_camera(char **words, int nb_words, t_data *data);

t_img_ppm	*ft_load_ppm_image(char *filename);
void		ft_log_ppm_image(t_img_ppm *img);


#endif