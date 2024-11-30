#include "ft_push_swap.h"

// Returns the index of the two elements with smallest <n>
// in the linked list stax->start[0]
void	compute_2mins(t_stax *stax, int *min_inds)
{
	int		ind;
	int		min_vals[2];
	t_rlist	*current;

	min_inds[0] = -1;
	min_inds[1] = -1;
	ind = 0;
	current = stax->start[0];
	while (ind < stax->size[0])
	{
		if (min_inds[0] == -1 || current->n < min_vals[0])
		{
			min_inds[1] = min_inds[0];
			min_vals[1] = min_vals[0];
			min_inds[0] = ind;
			min_vals[0] = current->n;
		}
		else if (min_inds[1] == -1 || current->n < min_vals[1])
		{
			min_inds[1] = ind;
			min_vals[1] = current->n;
		}
		current = current->next;
		ind++;
	}
}