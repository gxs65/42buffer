#include "ft_push_swap.h"

// Selector of the small stack sorting function to be applied
// (<l> indicates if it is stack A or B that must be sorted)
int	sort_smallstack(t_stax *stax, int l)
{
	int	ret;

	if (stax->size[0] <= 1)
		ret = 0;
	else if (stax->size[0] <= 2)
		ret = sort_stack2(&stax, l);
	else if (stax->size[0] <= 3)
		ret = sort_stack3(&stax, l);
	else
		ret = sort_stack5(&stax, l);
	return (ret);
}

// Sorts a stack of 2 elements in max 1 op, using swap
int	sort_stack2(t_stax *stax, int l)
{
	if (stax->start[l]->n > stax->start[l]->next->n)
		swap(stax, l);
	return (0);
}

// Sorts a stack of 3 elements in max 2 ops,
// 		making ops depending on the 6 possible configs for 3 numbers
// logline : ft_printf(LOGS, "%d %d %d\n", lst->n, lst->next->n, lst->prev->n);
int	sort_stack3(t_stax *stax, int l)
{
	t_rlist	*lst;

	lst = stax->start[l];
	if (lst->n >= lst->next->n && lst->n >= lst->prev->n)
		rotate(stax, l);
	else if (lst->next->n >= lst->n && lst->next->n > lst->prev->n)
		revrotate(stax, l);
	lst = stax->start[l];
	if (lst->n >= lst->next->n && lst->n <= lst->prev->n)
		swap(stax, l);
	return (0);
}

void	extract_2mins(t_stax *stax)
{
	int		ind;
	int		min_inds[2];
	int		min_vals[2];
	t_rlist	*current;

	min_inds[0] = -1;
	min_inds[1] = -1;
	ind = 0;
	current = stax->start[0];
	while (ind < stax->size[0])
	{
		if (current->n < min_vals[0])
		{
			min_inds[1] = min_inds[0];
			min_vals[1] = min_vals[0];
			min_inds[0] = ind;
			min_vals[0] = current->n;
		}
		else if (current->n < min_vals[1])
		{
			min_inds[1] = ind;
			min_vals[1] = current->n;
		}
		current = current->next;
		ind++;
	}
}

// Sorts a stack of 5 elements in max 12 ops,
//		using other smallstack and fcost sorting functions
// (does not take <l> into account, always sorts stack A)
int	sort_stack5(t_stax *stax, int l)
{
	

	(void)l;
	sort_smallstack(stax, 0);
	sort_smallstack(stax, 1);


	


	return (0);
}