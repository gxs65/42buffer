#include "../../h_files/ft_minirt.h"

// Performs the quadratic solving for the intersection between a ray and a sphere,
// 		returns -1 if the given ray does not intersects the sphere,
// 		returns the value of the ray scalar of intersection point if it does
// 			(the smallest of the 2 possible values if it intersects twice)
double	ft_intersect_ray_sphere2(t_ray ray, t_object *sphere, t_interval *hit_interval)
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

// Checks for intersection between ray and sphere,
// 		if so, fills <intersect> with the intersection informations, mainly the normal 
int	ft_intersect_ray_sphere(t_ray ray, t_object *sphere, t_interval *hit_interval, t_intersection *intersect)
{
	double	solution;
	
	solution = ft_intersect_ray_sphere2(ray, sphere, hit_interval);
	if (solution == -1)
		return (0);
	intersect->crosspoint = ft_ray_at(ray, solution);
	intersect->object = sphere;
	intersect->t = solution;
	intersect->normal = ft_vec_setnorm(ft_vec_substr(intersect->crosspoint, sphere->start), 1);
	return (1);
}

// Performs the linear solving for the intersection between ray and plane
// using the names :
// 		\ for the ray : s is origin vector, d is direction vector
// 		\ for the plane : p is a point on the plane, n is the plane's normal vector
// if the ray is not parallel to the plane (<=> n.d == 0),
// it must have a point on the plane, given by equation : n . ((s + t d) - p) == 0
// 		which yields the unique solution : t = (n.p - n.s) / n.d
// 			(the case of a division by 0 is precisely the case of parallelism)
int	ft_intersect_ray_plane2(t_ray ray, t_object *plane, t_interval *hit_interval)
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
		return (-1);
	solution = (np - ns) / nd;
	if (ft_interval_belongs(hit_interval, solution))
		return (solution);
	else
		return (-1);
}

// Checks for intersection between ray and plane,
// 		if so, fills <intersect> with the intersection informations, mainly the normal 
int	ft_intersect_ray_plane(t_ray ray, t_object *plane, t_interval *hit_interval, t_intersection *intersect)
{
	double	solution;

	solution = ft_intersect_ray_plane2(ray, plane, hit_interval);
	if (solution == -1)
		return (0);
	intersect->crosspoint = ft_ray_at(ray, solution);
	intersect->object = plane;
	intersect->t = solution;
	intersect->normal = ft_vec_copy(plane->normal);
	return (1);
}

// Performs the linear solving for the intersection between ray and cylinder
// using the names :
// 		\ cylinder : p a point on the spine, v the spine's direction vector, r the radius
// 		\ ray : s the origin point (camera), d the direction vector
// 		\ intermediaries : c is a candidate point, that could be on the cylinder or not
// -> Then the ray is expressed as `c = s + t1 * d` and the cylinder as `||c - (p + t2 * v)|| = r`
// 		where t2 is the scalar producing the spine point that is closest to the candidate point c
// -> we use the line/plane intersection logic to find the expression of t2 according to t1 :
// 		`t2 = v . c - v . p`
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

	if (ft_ops_solve_quadratic(a, b, c, solutions))
		return (-1);
	else
		return (1);
}

// Uses the same expression of the cylinder `||c - (p + t2 * v)|| = r`
// 		with `t2 = v.c - v.p`
// to determine if the given point lies inside the cylinder
int	ft_point_inside_cylinder(t_vec candidate, t_object *cylinder)
{
	double	axis_scalar;
	t_vec	closest_axis_point;
	double	distance_to_axis;

	axis_scalar = ft_vec_sproduct(cylinder->normal, candidate)
		- ft_vec_sproduct(cylinder->normal, cylinder->start);
	closest_axis_point = ft_vec_add(cylinder->start,
		ft_vec_scale(cylinder->normal, axis_scalar));
	distance_to_axis = ft_vec_norm(ft_vec_substr(candidate, closest_axis_point));
	if (distance_to_axis <= cylinder->dist)
		return (1);
	else
		return (0);
}

int	ft_intersect_ray_cylinder_sides(t_ray ray, t_object *cylinder, t_interval *hit_interval, t_intersection *intersect)
{
	t_intersection	intersect_sides[2];

	intersect_sides[0].t = INFINITE_DOUBLE;
	intersect_sides[1].t = INFINITE_DOUBLE;
	if (ft_point_inside_cylinder(ray.orig, cylinder))
	{
		if (!ft_intersect_ray_plane(ray, cylinder->sides[0], hit_interval, &(intersect_sides[0]))
			&& !ft_intersect_ray_plane(ray, cylinder->sides[1], hit_interval, &(intersect_sides[1])))
			return (0);
		if (intersect_sides[0].t < intersect_sides[1].t)
			*intersect = intersect_sides[0];
		else
			*intersect = intersect_sides[1];
		return (1);
	}
	return (0);
}

int	ft_intersect_ray_cylinder(t_ray ray, t_object *cylinder, t_interval *hit_interval, t_intersection *intersect)
{
	double	solutions[2];
	double	solution;
	double	axis_scalar;
	
	if (g_log)
		printf("Computing intersection between cylinder %p and ray\n", cylinder);
	if (ft_intersect_ray_cylinder2(cylinder->start, cylinder->normal, cylinder->dist,
		ray.orig, ray.dir, solutions) == -1) // 0 intersections -> ray is either outside of cylinder, or INSIDE and parallel
		return (ft_intersect_ray_cylinder_sides(ray, cylinder, hit_interval, intersect));
	else if (ft_interval_belongs(hit_interval, solutions[0]))
	{
		
	}
		solution = solutions[0];
	else if (ft_interval_belongs(hit_interval, solutions[1]))
		solution = solutions[1];
	else
		return (-1);
	intersect->crosspoint = ft_ray_at(ray, solutions[0]);
	axis_scalar = ft_vec_sproduct(cylinder->normal, intersect->crosspoint)
		- ft_vec_sproduct(cylinder->normal, cylinder->start);
	if (g_log)
		printf("\t\tSolution found : Cylinder hit at axis scalar %f\n", axis_scalar);
	if (ft_fabs(axis_scalar) < cylinder->height / 2)
	{
		return (solutions[0]);
	}
}


// Returns -1 if the given ray and object never intersect,
// 		and the scalar <t> to apply to ray to get intersection point if they do
int	ft_intersect_ray_object(t_ray ray, t_object *object, t_interval *hit_interval, t_intersection *intersect)
{
	if (object->type == SPHERE)
		return (ft_intersect_ray_sphere(ray, object, hit_interval, intersect));
	else if (object->type == PLANE)
		return (ft_intersect_ray_plane(ray, object, hit_interval, intersect));
	else if (object->type == CYLINDER)
		return (ft_intersect_ray_cylinder(ray, object, hit_interval, intersect));
	else
		return (-1);
}
