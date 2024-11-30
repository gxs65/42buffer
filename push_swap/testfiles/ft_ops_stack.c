#include "ft_push_swap.h"

// Functions to apply the operations on stacks A and B
// allowed by the subject : sa, sb, ss ; ra, rb, rr ; rra, rrb, rrr ; pa, pb

// SWAP stack
// 		- 1 element in the list : do nothing, return that element
// 		- 2 elements in the list : do nothing, return other element (<=> rr)
// 		- n>2 elements in the list : arrange the 6 links, return next element
t_rlist	*swap_rlist(t_rlist *elem1)
{
	t_rlist	*elem2;

	if (!elem1)
		return (NULL);
	elem2 = elem1->next;
	if (elem2 == elem1)
		return (elem1);
	if (elem1->prev == elem1->next)
		return (elem2);
	elem1->next = elem2->next;
	elem2->prev = elem1->prev;
	(elem1->prev)->next = elem2;
	(elem2->next)->prev = elem1;
	elem1->prev = elem2;
	elem2->next = elem1;
	return (elem2);
}

// Wrapper/printer for swapping
void	swap(t_stax *stax, int l)
{
	if (l == 0 || l == 1)
	{
		stax->start[l] = swap_rlist(stax->start[l]);
		ft_printf(OPS, "s%c\n", 'a' + l);
	}
	else
	{
		stax->start[0] = swap_rlist(stax->start[0]);
		stax->start[1] = swap_rlist(stax->start[1]);
		ft_printf(OPS, "ss\n");
	}
}

// ROTATE stack
// 		<=> shifting elements to the left : 1st elem becomes last elem
t_rlist *rotate_rlist(t_stax *stax, int l)
{
	if (!stax->start[l])
		return (NULL);
	if (stax->min[l] == 0)
		stax->min[l] = stax->size[l] - 1;
	else
		stax->min[l]--;
	return (stax->start[l]->next);
}

// Wrapper/printer for rotating
void	rotate(t_stax *stax, int l)
{
	if (l == 0 || l == 1)
	{
		stax->start[l] = rotate_rlist(stax, l);
		ft_printf(OPS, "r%c\n", 'a' + l);
	}
	else
	{
		stax->start[0] = rotate_rlist(stax, 0);
		stax->start[1] = rotate_rlist(stax, 1);
		ft_printf(OPS, "rr\n");
	}
}

// REVERSE ROTATE stack
// 		<=> shifting elements to the right : 1st elem becomes 2nd elem
t_rlist *revrotate_rlist(t_stax *stax, int l)
{
	if (!stax->start[l])
		return (NULL);
	if (stax->min[l] == stax->size[l] - 1)
		stax->min[l] = 0;
	else
		stax->min[l]++;
	return (stax->start[l]->prev);
}

// Wrapper/printer for revrotating
void	revrotate(t_stax *stax, int l)
{
	if (l == 0 || l == 1)
	{
		stax->start[l] = revrotate_rlist(stax, l);
		ft_printf(OPS, "rr%c\n", 'a' + l);
	}
	else
	{
		stax->start[0] = revrotate_rlist(stax, 0);
		stax->start[1] = revrotate_rlist(stax, 1);
		ft_printf(OPS, "rrr\n");
	}
}

// PUSH from stack src to stack dest
// 		- if no elements in src, do nothing
// 		- if 1 element in src, start_src becomes NULL
// 		- if n>2 elements in src, start_src becomes start_src->next
void	push_rlist(t_rlist **start_src, t_rlist **start_dest)
{
	t_rlist	*moving_elem;
	t_rlist	*next_src;

	if (!(*start_src))
		return ;
	if ((*start_src)->next == *start_src)
		next_src = NULL;
	else
		next_src = (*start_src)->next;
	moving_elem = ft_lstdelone(*start_src);
	*start_src = next_src;

	ft_lstadd_before(start_dest, moving_elem);
}

// Wrapper/printer for pushing
void	push(t_stax *stax, int l)
{
	push_rlist(&(stax->start[1 - l]), &(stax->start[l]));
	stax->size[1 - l]--;
	stax->size[l]++;
	ft_printf(OPS, "p%c\n", 'a' + l);
}