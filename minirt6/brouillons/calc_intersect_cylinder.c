#include "../h_files/ft_minirt.h"

double	ft_solve_intersect_line_cylinder(t_vec p, t_vec v, double r, t_vec s, t_vec d)
{
	double	dot_vd;
	double	dot_vs_vp;
	double	ax;
	double	ay;
	double	az;
	double	bx;
	double	by;
	double	bz;
	double	cx;
	double	cy;
	double	cz;
	double	a;
	double	b;
	double	c;
	double	solutions[2];
	int		ret;

	dot_vd = ft_vec_sproduct(d, v);
	dot_vs_vp = ft_vec_sproduct(v, s) - ft_vec_sproduct(v, p);
	ax = (dot_vd * v.x + d.x) * (dot_vd * v.x + d.x);
	ay = (dot_vd * v.y + d.y) * (dot_vd * v.y + d.y);
	cy = (dot_vd * v.z + d.z) * (dot_vd * v.z + d.z);
	a = ax + ay + az;

	bx = 2 * ((v.x * dot_vd) * (s.x - p.x + dot_vs_vp * v.x) + d.x * (dot_vs_vp * v.x + s.x - p.x));
	by = 2 * ((v.y * dot_vd) * (s.y - p.y + dot_vs_vp * v.y) + d.y * (dot_vs_vp * v.y + s.y - p.y));
	bz = 2 * ((v.z * dot_vd) * (s.z - p.z + dot_vs_vp * v.z) + d.z * (dot_vs_vp * v.z + s.z - p.z));
	b = bx + by + bz;

	cx = (s.x - p.x) * (s.x - p.x) + dot_vs_vp * v.x * (2 * (s.x - p.x) + dot_vs_vp * v.x);
	cy = (s.y - p.y) * (s.y - p.y) + dot_vs_vp * v.y * (2 * (s.y - p.y) + dot_vs_vp * v.y);
	cx = (s.z - p.z) * (s.z - p.z) + dot_vs_vp * v.z * (2 * (s.z - p.z) + dot_vs_vp * v.z);
	c = cx + cy + cz;

	ret = ft_ops_solve_quadratic(a, b, c, solutions);
	if (ret)
		printf("No solutions found\n");
	else
		printf("Solution 1 : %f, solution 2 : %f\n", solutions[0], solutions[1]);
	return (0.0);
}

int	main(void)
{
	t_vec	s;
	t_vec	d;
	t_vec	p;
	t_vec	v;

	p = ft_vec_init(0, 4, 0);
	v = ft_vec_init(0, 0, 1);
	s = ft_vec_init(0, 0, 0);
	d = ft_vec_init(0, 1, 0);
	ft_solve_intersect_line_cylinder(p, v, 1, s, d);
}
