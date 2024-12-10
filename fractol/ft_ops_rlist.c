#include "ft_fractol.h"

// Functions to manipulate a linked list of <t_rlist>
// 		- cyclical linked list with double direction
// 		- content : a pixel with integer coordinates
// 		- used in fractal drawing functions to emulate a queue
// File 1 : Adding and removing elements

// Creation of a new element
// 		(initialized like a 1-element list : <prev> and <next> point to itself)
t_rlist	*ft_lstnew(int x, int y)
{
	t_rlist	*newelem;

	newelem = malloc(1 * sizeof(t_rlist));
	if (!newelem)
		return ((t_rlist *) 0);
	newelem->x = x;
	newelem->y = y;
	newelem->next = newelem;
	newelem->prev = newelem;
	return (newelem);
}

// Insertion of an element just before the element <lst>
// REPLACING the element <lst> as first element of the list
// 		(if <lst> was alone, ie has <prev> pointing to itself,
// 		then the <next> and <prev> of the new element both point to <lst>)
void	ft_lstadd_front(t_rlist **lst, t_rlist *newelem)
{
	if (*lst)
	{
		newelem->next = *lst;
		if ((*lst)->prev == *lst)
		{
			newelem->prev = *lst;
			(*lst)->next = newelem;
		}
		else
		{
			newelem->prev = (*lst)->prev;
			(*lst)->prev->next = newelem;
		}
		(*lst)->prev = newelem;
	}
	*lst = newelem;
}

// Insertion of an element just before the element <lst>
// NOT REPLACING the element <lst> as first element of the list
// -> since list is cyclical, the element is added at the end of list
// 		(if <lst> was alone, ie has <prev> pointing to itself,
// 		then the <next> and <prev> of the new element both point to <lst>)
void	ft_lstadd_back(t_rlist **lst, t_rlist *newelem)
{
	if (*lst)
	{
		newelem->next = *lst;
		if ((*lst)->prev == *lst)
		{
			newelem->prev = *lst;
			(*lst)->next = newelem;
		}
		else
		{
			newelem->prev = (*lst)->prev;
			(*lst)->prev->next = newelem;
		}
		(*lst)->prev = newelem;
	}
	else
		*lst = newelem;
}

// Removal of 1 element from the list, replacing the links to maintain the rest
// 		/!\ removed element is freed
// (if list had only 2 elements, the <prev> and <next> of the last element
// 	will be replaced correctly so that they point to that same last element)
void	ft_lstdelone(t_rlist *lst)
{
	if (lst)
	{
		if (lst->next != lst)
		{
			lst->prev->next = lst->next;
			lst->next->prev = lst->prev;
		}
		lst->next = lst;
		lst->prev = lst;
	}
	free(lst);
}

// Deletion of all the elements in the list
void	ft_lstclear(t_rlist *lst)
{
	if (lst)
	{
		lst->prev->next = NULL;
		ft_lstclear(lst->next);
		free(lst);
	}
}

// Count of elements of the linked, which requires to keep <orig> known,
// 		since the list is cyclical -> the end cond is <elem> == <orig>
int	ft_lstsize(t_rlist *elem, t_rlist *orig, int passed_first)
{
	if (elem && !(elem == orig && passed_first))
		return (1 + ft_lstsize(elem->next, orig, 1));
	else
		return (0);
}
