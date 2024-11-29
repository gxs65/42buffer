#include "ft_push_swap.h"

// Searches a the <rlist> indexed <l> for the smallest element that is still >n
// 		-> the given <rlist> will be sorted but not starting with min,
// 		   so first loop to get to min, then second loop to search
// (returns -1 if the list was looped through without finding
//  anything bigger than <n>)
int	find_smallest_elem_above(t_stax *stax, int l, int n)
{
	int		ind;
	t_rlist	*current;

	current = stax->start[l];
	ind = 0;
	while (ind < stax->min[l])
	{
		ind++;
		current = current->next;
	}
	ind = 0;
	while (ind < stax->size[l])
	{
		if (current->n >= n)
			return ((stax->min[l] + ind) % stax->size[l]);
		ind++;
		current = current->next;
	}
	return (-1);
}

void	compute_cost(t_stax *stax, t_cost *cost, t_rlist *current, int ind)
{
	cost->indB = ind;
	cost->rb = ind;
	cost->rrb = stax->size[1] - ind;
	ft_printf(LOGS, "\tFinding index to fetch in stack A...\n");
	cost->indA = find_smallest_elem_above(stax, 0, current->n);
	ft_printf(LOGS, "\tFound index to fetch in stack A : %d\n",  cost->indA);
	if (cost->indA == -1)
		cost->indA = stax->min[0];
	cost->ra = cost->indA;
	cost->rra = stax->size[0] - cost->indA;
	cost->best_mode = ind_min_4args(max(cost->ra, cost->rb),
		max(cost->rra, cost->rrb), cost->ra + cost->rrb, cost->rra + cost->rb);
	cost->total = min_nargs(4, max(cost->ra, cost->rb),
		max(cost->rra, cost->rrb), cost->ra + cost->rrb, cost->rra + cost->rb);
}

void	display_cost(t_cost *cost)
{
	ft_printf(LOGS, "COST : ra %d, rb %d, rra %d, rrb %d, indA %d, indB %d\n",
		cost->ra, cost->rb, cost->rra, cost->rrb, cost->indA, cost->indB);
}

void	apply_cost(t_cost *cost, t_stax *stax)
{
	int	ind;

	display_cost(cost);
	if (cost->best_mode == 0)
	{
		ind = 0;
		while (ind < min(cost->ra, cost->rb))
		{
			ind++;
			rotate(stax, 2);
		}
		while (ind < max(cost->ra, cost->rb))
		{
			ind++;
			rotate(stax, cost->ra < cost->rb);
		}
	}
	if (cost->best_mode == 1)
	{
		ind = 0;
		while (ind < min(cost->rra, cost->rrb))
		{
			revrotate(stax, 2);
			ind++;
		}
		while (ind < max(cost->rra, cost->rrb))
		{
			revrotate(stax, cost->rra < cost->rrb);
			ind++;
		}
	}
	if (cost->best_mode == 2)
	{
		ind = 0;
		while (ind < cost->ra)
		{
			rotate(stax, 0);
			ind++;
		}
		ind = 0;
		while (ind < cost->rrb)
		{
			revrotate(stax, 1);
			ind++;
		}
	}
	if (cost->best_mode == 3)
	{
		ind = 0;
		while (ind < cost->rra)
		{
			revrotate(stax, 0);
			ind++;
		}
		ind = 0;
		while (ind < cost->rb)
		{
			rotate(stax, 1);
			ind++;
		}
	}
	push(stax, 0);
	//ft_printf(LOGS, "elem0 %d, elem1 %d\n", stax->start[0]->n, stax->start[0]->next->n);
	if (stax->size[0] > 1 && (stax->min[0] != 0
		|| stax->start[0]->n > stax->start[0]->next->n))
		stax->min[0]++;
}

void	fetch_minimum(t_stax *stax, int l)
{
	if (stax->min[l] > stax->size[l] / 2)
	{
		while (stax->min[l] != 0)
			revrotate(stax, l);
	}
	else
	{
		while (stax->min[l] != 0)
			rotate(stax, l);
	}
}

void	initial_push(t_stax *stax)
{
	int	*tab_stackA;
	int	*seq;
	int	seq_size;
	int	ind_in_seq;
	int	ind_in_lst;

	ft_printf(LOGS, "\n=== INITIAL PUSH ===\n");
	tab_stackA = ft_rlist_to_array(stax->start[0]);
	ft_printf(LOGS, "Input in array form: ");
	print_int_tab(tab_stackA, stax->size[0]);
	ft_printf(LOGS, "[Search for longest ascending seq] Dyn_result and Mem_previous :\n");
	seq = longest_ascending_seq(tab_stackA, stax->size[0], &seq_size);
	ft_printf(LOGS, "--> Sequence longest ascending : ");
	print_int_tab(seq, seq_size);

	ind_in_seq = 0;
	ind_in_lst = 0;
	while (stax->size[0] > seq_size)
	{
		if (ind_in_lst == seq[ind_in_seq])
		{
			ind_in_seq++;
			rotate(stax, 0);
		}
		else
			push(stax, 1);
		ind_in_lst++;
	}
	stax->min[0] = 0;
}

int	sort_fcost(t_stax *stax)
{
	t_cost	cost;
	t_cost	min_cost;
	int		ind;
	t_rlist	*current;
	int		total_ops; // remove
	total_ops = 0;

	initial_push(stax);
	display_stacks(stax->start[0], stax->start[1], 0);
	ft_printf(LOGS, "--- siz A = %d, siz B = %d\n", stax->size[0], stax->size[1]);
	ft_printf(LOGS, "--- min A = %d, min B = %d\n", stax->min[0], stax->min[1]);
	ft_printf(LOGS, "\n=== MAIN INSERT ===\n");

	while (stax->size[1])
	{
		initialize_cost(&cost);
		initialize_cost(&min_cost);
		ind = 0;
		current = stax->start[1];
		while (ind < stax->size[1])
		{
			ft_printf(LOGS, "Computing cost for [element stack B index %d number %d]...\n", ind, current->n);
			compute_cost(stax, &cost, current, ind);
			if (min_cost.total == -1 || cost.total < min_cost.total)
				copy_cost(&cost, &min_cost);
			ft_printf(LOGS, "Cost for [element stack B index %d number %d] : total %d with mode %d\n", ind, current->n, cost.total, cost.best_mode);
			ft_printf(LOGS, "Current min cost : index %d, total %d with mode %d\n", min_cost.indB, min_cost.total, min_cost.best_mode);
			ind++;
			current = current->next;
		}
		ft_printf(LOGS, "Applying cost : elem ind %d moved to stack A...\n", min_cost.indB);
		total_ops = total_ops + min_cost.total + 1;
		apply_cost(&min_cost, stax);
		display_stacks(stax->start[0], stax->start[1], 0);
		ft_printf(LOGS, "--- siz A = %d, siz B = %d\n", stax->size[0], stax->size[1]);
		ft_printf(LOGS, "--- min A = %d, min B = %d\n", stax->min[0], stax->min[1]);
	}
	fetch_minimum(stax, 0);
	ft_printf(SORTED, "After final min fetching :\n");
	display_stacks(stax->start[0], stax->start[1], SORTED);
	ft_printf(SORTED, "========== TOTAL_OPS (not counting initial push + final fetch): %d\n", total_ops);
	return (0);
}

