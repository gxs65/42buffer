#include "../../h_files/ft_minirt.h"

int	ft_ops_solve_quadratic(double a, double b, double c, double *solutions)
{
	double	discriminant;

	discriminant = b * b - 4 * a * c;
	if (a == 0 || discriminant < 0)
	{
		return (1);
	}
	solutions[0] = (-1 * b - sqrt(discriminant)) / (2 * a);
	solutions[1] = (-1 * b + sqrt(discriminant)) / (2 * a);
	return (0);
}