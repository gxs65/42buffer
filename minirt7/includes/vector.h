/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:34:50 by alex              #+#    #+#             */
/*   Updated: 2025/03/13 19:40:58 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

// Vector operations
t_vec		ft_vec_copy(t_vec v);
t_vec		ft_vec_init(double x, double y, double z);
t_vec		ft_vec_add(t_vec v1, t_vec v2);
t_vec		ft_vec_neg(t_vec v);
t_vec		ft_vec_substr(t_vec v1, t_vec v2);
t_vec		ft_vec_scale(t_vec v, double a);
t_vec		ft_vec_scale_by_vec(t_vec v1, t_vec v2);
double		ft_vec_norm_sq(t_vec v);
double		ft_vec_norm(t_vec v);
t_vec		ft_vec_setnorm(t_vec v, double new_norm);
double		ft_vec_sproduct(t_vec v1, t_vec v2);
t_vec		ft_vec_vproduct(t_vec v1, t_vec v2);
void		ft_vec_display(t_vec v, int newline);
t_vec		ft_vec_init_random_within(double min, double max);
t_vec		ft_vec_init_random_unit(void);
int			ft_vec_is_infinitesimal(t_vec v);
t_vec		ft_vec_symmetric_by_normal(t_vec incident, t_vec normal);
int			ft_vec_from_str(t_vec *vec_dest, char *s);
// Rays operations
t_vec		ft_ray_at(t_ray ray, double p);
t_ray		ft_ray_init_by_point(t_vec orig, t_vec dir);
t_ray		ft_ray_init_by_dir(t_vec orig, t_vec dir);
void		ft_ray_display(t_ray ray, int newline);
// Standard mathematics
int			ft_ops_solve_quadratic(double a, double b, double c, double *solutions);
// Intervals
t_interval	ft_interval_init(double min, double max);
int			ft_interval_belongs(t_interval *interval, double d);
// Others
double		ft_fmin(double a, double b);
double		ft_fmax(double a, double b);
double		ft_degrees_to_radians(double d);
double		ft_radians_to_degrees(double r);
double		ft_rand_double(void);
double		ft_rand_double_within(double min, double max);
double		ft_fabs(double a);
double		ft_atof(char *s);
long		ft_power(int base, int exponent);

#endif