/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:34:50 by alex              #+#    #+#             */
/*   Updated: 2025/03/20 21:24:31 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

int			ft_init_parsing(t_data *data, char **av);
int			ft_parse_scene(char *scene_file, t_data *data);

int			ft_register_object_texture(char **words, int nb_words, int expected_nb_words, t_object *object);
int			ft_register_sphere(char **words, int nb_words, t_data *data);
int			ft_register_plane(char **words, int nb_words, t_data *data);
int			ft_register_cylinder(char **words, int nb_words, t_data *data);
int			ft_register_cone(char **words, int nb_words, t_data *data);

int			ft_register_alightning(char **words, int nb_words, t_data *data);
int			ft_register_lamp(char **words, int nb_words, t_data *data);
int			ft_register_camera(char **words, int nb_words, t_data *data);

int			ft_init_textures(t_data *data);
t_img_ppm	*ft_load_ppm(char *basename);
int			ft_read_ppm_header(int fd, int *width, int *height);
int			ft_count_words(char **words);
void		ft_delete_ending_linebreak(char *line);

void		ft_free_strarray_nullterm(char **strs);
void		ft_free_2dim_doublearray_upto(double **arr, int	upto);
double		**ft_allocate_2dim_doublearray(int height, int width);
void		ft_free_2dim_vecarray_upto(t_vec **arr, int	upto);
t_vec		**ft_allocate_2dim_vecarray(int height, int width);

// Utils
double		ft_atof(char *s);
void		ft_log_scene(t_data *data);
void		ft_log_objects(t_object *current);
void		ft_log_textures(t_img_ppm **textures, int nb);
void		ft_log_ppm(t_img_ppm *img);


#endif