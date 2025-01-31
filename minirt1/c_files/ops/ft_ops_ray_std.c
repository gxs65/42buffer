#include "../../h_files/ft_minirt.h"

t_vec	ft_ray_at(t_ray *ray, double p)
{
	t_vec	res;

	res = ft_vec_scale(ray->dir, p);
	res = ft_vec_add(res, ray->orig);
	return (res);
}