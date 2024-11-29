#include "ft_push_swap.h"

void	initialize_stax(t_stax *stax)
{
	stax->start[0] = NULL;
	stax->start[1] = NULL;
	stax->size[0] = 0;
	stax->size[1] = 0;
	stax->min[0] = 0;
	stax->min[1] = 0;
}

void	initialize_cost(t_cost *cost)
{
	cost->ra = 0;
	cost->rb = 0;
	cost->rra = 0;
	cost->rrb = 0;
	cost->total = -1;
	cost->best_mode = -1;
	cost->indA = -1;
	cost->indB = -1;
}

void	copy_cost(t_cost *src, t_cost *dest)
{
	dest->ra = src->ra;
	dest->rb = src->rb;
	dest->rra = src->rra;
	dest->rrb = src->rrb;
	dest->total = src->total;
	dest->best_mode = src->best_mode;
	dest->indA = src->indA;
	dest->indB = src->indB;
}

// Minimum of 2 integers (returns a if a==b)
int	min(int a, int b)
{
	if (a > b)
		return (b);
	else
		return (a);
}

// Maximum of 2 integers (returns a if a==b)
int	max(int a, int b)
{
	if (a < b)
		return (b);
	else
		return (a);
}

int	min_nargs_recurr(int n, va_list ap)
{
	int	prov_min;
	int	a;

	if (n == 0)
		return (-1);
	a = va_arg(ap, int);
	prov_min = min_nargs_recurr(n -1, ap);
	if (prov_min == -1)
		return (a);
	else
		return (min(a, prov_min));
}

// Minimum of any number of arguments, with variadic arguments
// (only for positive numbers)
int	min_nargs(int n, int a, int b, ...)
{
	va_list	ap;
	int		prov_min;

	va_start(ap, b);
	prov_min = min_nargs_recurr(n - 2, ap);
	va_end(ap);
	if (prov_min == -1)
		return (min(a, b));
	else
		return (min(prov_min, min(a, b)));
}

// f : use malloc and cariadic arguments for the translation to array
// Very ugly ad hoc function to return the index of the min of 4 numbers
int	ind_min_4args(int a, int b, int c, int d)
{
	int	tab[4];
	int	ind;
	int	min_ind;

	tab[0] = a;
	tab[1] = b;
	tab[2] = c;
	tab[3] = d;
	ind = 1;
	min_ind = 0;
	while (ind < 4)
	{
		if (tab[ind] < tab[min_ind])
			min_ind = ind;
		ind++;
	}
	return (min_ind);
}
