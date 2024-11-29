#include "ft_push_swap.h"

// Displays elements of the list, if the <;ode> is not 0
void	display_rlist(t_rlist *elem, t_rlist *orig, int looped, int mode)
{
	if (elem && !(elem == orig && looped))
	{
		looped = 1;
		if (!mode)
		{
			ft_printf(LOGS, "%d ", elem->n);
		}
		else
			ft_printf(1, "%d ", elem->n);
		//ft_printf(1, "\n[prev %p, here %p, n = %d, next %p]",
		// elem->prev, elem, elem->n, elem->next);
		display_rlist(elem->next, orig, looped, mode);
	}
}

// Wrapper for <display_rlist>
void	display_stacks(t_rlist *starta, t_rlist *startb, int mode)
{
	ft_printf(mode, "Stack A : ");
	display_rlist(starta, starta, 0, mode);
	ft_printf(mode, "\nStack B : ");
	display_rlist(startb, startb, 0, mode);
	ft_printf(mode, "\n");
}

// Integer array printer
void	print_int_tab(int *tab, int size)
{
	int	ind;

	ft_printf(LOGS, "(tab of size %d) ", size);
	ind = 0;
	while (ind < size)
	{
		ft_printf(LOGS, "%d ", tab[ind]);
		ind++;
	}
	ft_printf(LOGS, "\n");
}
