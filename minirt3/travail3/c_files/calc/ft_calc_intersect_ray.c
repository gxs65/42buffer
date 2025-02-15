#include "../../h_files/ft_minirt.h"

// Returns -1 if the given does not intersects the sphere,
// 		and the value of the ray scalar of intersection point if it does
// 			(the smallest of the 2 possible values if it intersects twice)
double	ft_intersect_ray_sphere(t_ray ray, t_object sphere, t_interval *hit_interval)
{
	double		a;
	double		b;
	double		c;
	double		solutions[2];

	//printf("Computing intersection between sphere (radius %f) and ray\n", sphere.dist);
	a = ft_vec_sproduct(ray.dir, ray.dir);
	b = 2 * ft_vec_sproduct(ray.dir, ft_vec_substr(ray.orig, sphere.start));
	c = ft_vec_sproduct(ray.orig, ray.orig) + ft_vec_sproduct(sphere.start, sphere.start)
		- 2 * ft_vec_sproduct(ray.orig, sphere.start) - (sphere.dist * sphere.dist);
	if (ft_ops_solve_quadratic(a, b, c, solutions))
		return (-1);
	else if (ft_interval_belongs(hit_interval, solutions[0]))
		return (solutions[0]);
	else if (ft_interval_belongs(hit_interval, solutions[1]))
		return (solutions[1]);
	else
		return (-1);
}

// Returns -1 if the given ray and object never intersect,
// 		and the scalar <t> to apply to ray to get intersection point if they do
double	ft_intersect_ray_object(t_ray ray, t_object object, t_interval *hit_interval)
{
	if (object.type == SPHERE)
		return (ft_intersect_ray_sphere(ray, object, hit_interval));
	else
		return (-1);
}
