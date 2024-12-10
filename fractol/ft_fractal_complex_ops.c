#include "ft_fractol.h"

// Computes the distance between two points with Pythagore
// (if <pb> is NULL, instead computes distance of <pa> to origin)
double	dist(t_point *pa, t_point *pb)
{
	double	x;
	double	y;

	if (pb)
	{
		x = fabs(pa->x - pb->x);
		y = fabs(pa->y - pb->y);
	}
	else
	{
		x = fabs(pa->x);
		y = fabs(pa->y);
	}
	return (sqrt(x * x + y * y));
}

// Same as <dist>, but yields the number without having computed its square root
double	dist_squared(t_point *pa, t_point *pb)
{
	double	x;
	double	y;

	if (pb)
	{
		x = fabs(pa->x - pb->x);
		y = fabs(pa->y - pb->y);
	}
	else
	{
		x = fabs(pa->x);
		y = fabs(pa->y);
	}
	return (x * x + y * y);
}

// Addition of 2 complex numbers : simply adding each (imaginary/real) part
void	add_complex(t_point *a, t_point *b, t_point *res)
{
	res->x = a->x + b->x;
	res->y = a->y + b->y;
}

// Product of 2 complex numbers : following the development
// 		(ax + ay * i)  *  (bx + by * i)
// =	ax * bx  +  ax * by * i  +  ay * i * bx   +  ay * i * by * i		| double development
// =	ax * bx  +  (ax * by + ay * bx) * i  +  ay * by * i**2				| where i**2 = -1
// =	(ax * bx - ay * by)  +  (ax * by + ay * bx) * i						| <=> real part and imaginary part
void	multiply_complex(t_point *a, t_point *b, t_point *res)
{
	res->x = a->x * b->x - a->y * b->y;
	res->y = a->y * b->x + a->x * b->y;
}

// Applies the simple quadratic map "z2 = z1**2 + c"
// with z1 given by parameter <p> and c given by parameter <c>
// /!\ Does the operation inplace, so <p> will now have the value of z2
//  	(-> function <multiply_complex> is called with <p> as both operands ;
// 			function <add_complex> is called with <p> as recipient of result)
void	apply_complex_qmap_inplace(t_point *p, t_point *c)
{
	t_point	tmp;

	multiply_complex(p, p, &tmp);
	add_complex(&tmp, c, p);
}

// Applies the simple quadratic map "z2 = z1**2 + c"
// with as few multiplications as possible, to reduce compute time
// (<tmp> used as a swapper)
void	apply_complex_qmap_inplace2(t_point *p, t_point *c)
{
	t_point	tmp;

	tmp.x = p->x;
	tmp.y = p->y;
	p->x = (tmp.x + tmp.y) * (tmp.x - tmp.y) + c->x; // realpart = (x + y)(x - y) + u ; "identite remarquable"
	p->y = (tmp.x + tmp.x) * (tmp.y) + c->y; // imaginarypart = 2xy + v ; 2x transformed into (x + x) to avoid multiply
}

// Applies the simple quadratic map defined by <c> to complex number <p>
// until the module of <p> (= distance to origin) goes beyond the <threshold>,
// 		then returns how many applications were performed
// (Max <max_iter> applications will be performed)
// (Apparently no need to ever calculate argument)
int	time_to_diverge(t_point *p, t_point *c, double threshold_sq, int max_iter)
{
	int		ind;
	double	module;

	ind = 1;
	module = -1;
	while (ind < max_iter && module <= threshold_sq)
	{
		apply_complex_qmap_inplace2(p, c);
		module = dist_squared(p, NULL);
		//printf("\tAfter %d iterations of qmap : p = %f + %f * i\n", ind, p->x, p->y); // LOG WITH PRINTF
		//printf("\t--- Module = %f, Arg = %f\n", module, arg); // LOG WITH PRINTF
		ind++;
	}
	if (ind == max_iter)
		return (-1);
	else
		return (ind);
}
