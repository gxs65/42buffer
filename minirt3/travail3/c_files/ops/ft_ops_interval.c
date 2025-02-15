#include "../../h_files/ft_minirt.h"

t_interval	ft_interval_init(double min, double max)
{
	t_interval	res;

	res.min = min;
	res.max = max;
	res.size = max - min;
	return (res);
}

int	ft_interval_belongs(t_interval *interval, double d)
{
	if (interval->min <= d && d <=interval->max)
		return (1);
	else
		return (0);
}
