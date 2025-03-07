#include "../../h_files/ft_minirt.h"

// Returns -1 if the given does not intersects the sphere,
// 		and the value of the ray scalar of intersection point if it does
// 			(the smallest of the 2 possible values if it intersects twice)
double	ft_intersect_ray_sphere(t_ray ray, t_object *sphere, t_interval *hit_interval)
{
	double		a;
	double		b;
	double		c;
	double		solutions[2];

	if (g_log)
		printf("Computing intersection between sphere %p (radius %f) and ray\n", sphere, sphere->dist);
	a = ft_vec_sproduct(ray.dir, ray.dir);
	b = 2 * ft_vec_sproduct(ray.dir, ft_vec_substr(ray.orig, sphere->start));
	c = ft_vec_sproduct(ray.orig, ray.orig) + ft_vec_sproduct(sphere->start, sphere->start)
		- 2 * ft_vec_sproduct(ray.orig, sphere->start) - (sphere->dist * sphere->dist);
	if (ft_ops_solve_quadratic(a, b, c, solutions))
		return (-1);
	else if (ft_interval_belongs(hit_interval, solutions[0]))
		return (solutions[0]);
	else if (ft_interval_belongs(hit_interval, solutions[1]))
		return (solutions[1]);
	else
		return (-1);
}

// With the names ;
// 		\ for the ray : s is origin vector, d is direction vector
// 		\ for the plane : p is a point on the plane, n is the plane's normal vector
// then, if the ray is not parallel to the plane (<=> n.d == 0),
// it must have a point on the plane, given by equation : n . ((s + t d) - p) == 0
// 		which yields the unique solution : t = (n.p - n.s) / n.d
// 			(the case of a division by 0 is precisely the case of parallelism)
double	ft_intersect_ray_plane(t_ray ray, t_object *plane, t_interval *hit_interval)
{
	double	np;
	double	ns;
	double	nd;
	double	solution;

	if (g_log)
		printf("Computing intersection between plane %p and ray\n", plane);
	np = ft_vec_sproduct(plane->normal, plane->start);
	ns = ft_vec_sproduct(plane->normal, ray.orig);
	nd = ft_vec_sproduct(plane->normal, ray.dir);
	if (ft_fabs(nd) < 1e-20)
	{
		if (g_log)
			printf("\tNo solution, the ray runs parallel to plane\n");
		return (-1);
	}
	solution = (np - ns) / nd;
	if (g_log)
		printf("\tSolution computed : %f\n", solution);
	if (ft_interval_belongs(hit_interval, solution))
		return (solution);
	else
		return (-1);
}

// Using the names :
// 		\ cylinder : p a point on the spine, v the spine's direction vector, r the radius
// 		\ ray : s the origin point (camera), d the direction vector
// 		\ intermediaries : c a candidate point, that could be on the cylinder or not
// -> Then the ray is expressed as `s + t1 * d` and the cylinder as `||c - (p + t2 * v)|| = r`
// 		where t2 is the scalar producing the spine point that is closest to the candidate point c
// -> we use the line/plane intersection logic to find the expression of t2 according to t1 : `t2 = v . c- v . p`
// -> we replace t2 inside the cylinder's definition, to obtain a quadratic equation on t1
// 		which has the a/b/c coefficients detailed here after development/factorisation
// (dot_vd and dot_vs_vp are only helper variables)
int	ft_intersect_ray_cylinder2(t_vec p, t_vec v, double r, t_vec s, t_vec d, double *solutions)
{
	double	dot_vd,	dot_vs_vp;
	double	ax, ay, az, bx, by, bz, cx, cy, cz;
	double	a, b, c;

	dot_vd = -1 * ft_vec_sproduct(d, v);
	dot_vs_vp = -1 * (ft_vec_sproduct(v, s) - ft_vec_sproduct(v, p));
	ax = (dot_vd * v.x + d.x) * (dot_vd * v.x + d.x);
	ay = (dot_vd * v.y + d.y) * (dot_vd * v.y + d.y);
	az = (dot_vd * v.z + d.z) * (dot_vd * v.z + d.z);
	a = ax + ay + az;

	bx = 2 * ((v.x * dot_vd) * (s.x - p.x + dot_vs_vp * v.x) + d.x * (dot_vs_vp * v.x + s.x - p.x));
	by = 2 * ((v.y * dot_vd) * (s.y - p.y + dot_vs_vp * v.y) + d.y * (dot_vs_vp * v.y + s.y - p.y));
	bz = 2 * ((v.z * dot_vd) * (s.z - p.z + dot_vs_vp * v.z) + d.z * (dot_vs_vp * v.z + s.z - p.z));
	b = bx + by + bz;

	cx = (s.x - p.x) * (s.x - p.x) + dot_vs_vp * v.x * (2 * (s.x - p.x) + dot_vs_vp * v.x);
	cy = (s.y - p.y) * (s.y - p.y) + dot_vs_vp * v.y * (2 * (s.y - p.y) + dot_vs_vp * v.y);
	cz = (s.z - p.z) * (s.z - p.z) + dot_vs_vp * v.z * (2 * (s.z - p.z) + dot_vs_vp * v.z);
	c = cx + cy + cz - r * r;

	return (ft_ops_solve_quadratic(a, b, c, solutions));
}

double	ft_intersect_ray_cylinder(t_ray ray, t_object *cylinder, t_interval *hit_interval)
{
	t_vec	s;
	t_vec	d;
	t_vec	p;
	t_vec	v;
	int		ret;
	double	solutions[2];
	double	axis_scalar;
	t_vec	crosspoint;

	if (g_log)
		printf("Computing intersection between cylinder %p and ray\n", cylinder);
	p = cylinder->start;
	v = cylinder->normal;
	s = ray.orig;
	d = ray.dir;
	ret = ft_intersect_ray_cylinder2(p, v, cylinder->dist, s, d, solutions);
	if (ret)
		return (-1);
	if (ft_interval_belongs(hit_interval, solutions[0]))
	{
		crosspoint = ft_ray_at(ray, solutions[0]);
		axis_scalar = ft_vec_sproduct(cylinder->normal, crosspoint) - ft_vec_sproduct(cylinder->normal, cylinder->start);
		if (g_log)
			printf("\t\tSolution 0 : Cylinder hit at axis scalar %f\n", axis_scalar);
		if (ft_fabs(axis_scalar) < cylinder->height / 2)
		{
			return (solutions[0]);
		}
	}
	if (ft_interval_belongs(hit_interval, solutions[1]))
	{
		crosspoint = ft_ray_at(ray, solutions[1]);
		axis_scalar = ft_vec_sproduct(cylinder->normal, crosspoint) - ft_vec_sproduct(cylinder->normal, cylinder->start);
		if (g_log)
			printf("\t\tSolution 1 : Cylinder hit at axis scalar %f\n", axis_scalar);
		if (ft_fabs(axis_scalar) < cylinder->height / 2)
		{
			return (solutions[1]);
		}
	}
	return (-1);
}


// Returns -1 if the given ray and object never intersect,
// 		and the scalar <t> to apply to ray to get intersection point if they do
double	ft_intersect_ray_object(t_ray ray, t_object *object, t_interval *hit_interval)
{
	if (object->type == SPHERE)
		return (ft_intersect_ray_sphere(ray, object, hit_interval));
	else if (object->type == PLANE)
		return (ft_intersect_ray_plane(ray, object, hit_interval));
	else if (object->type == CYLINDER)
		return (ft_intersect_ray_cylinder(ray, object, hit_interval));
	else
		return (-1);
}
