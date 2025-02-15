#include "../../h_files/ft_minirt.h"

double	ft_fmin(double a, double b)
{
	if (a <= b)
		return (a);
	else
		return (b);
}

double	ft_fmax(double a, double b)
{
	if (a >= b)
		return (a);
	else
		return (b);
}

double	ft_degrees_to_radians(double d)
{
	return (d * PI / 180.0);
}

double	ft_radians_to_degrees(double r)
{
	return (r * 180.0 / PI);
}

double	ft_rand_double(void)
{
	long	rand_max_excluded;

	rand_max_excluded = (long)RAND_MAX + 1;
	return (rand() / (double)rand_max_excluded);
}

double	ft_rand_double_within(double min, double max)
{
	double n;

	n = ft_rand_double();
	return (min + (max - min) * n);
}

double	ft_fabs(double a)
{
	if (a < 0)
		return (a * -1);
	else
		return (a);
}
