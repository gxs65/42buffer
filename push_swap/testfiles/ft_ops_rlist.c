#include "ft_push_swap.h"

// Functions to manipulate a linked list of <t_rlist>
// 		(cyclical linked list with double direction)

// Creation of a new element
// 		(initialized like a 1-element list : <prev> and <next> point to itself)
t_rlist	*ft_lstnew(int n)
{
	t_rlist	*newelem;
	int		ind;

	newelem = malloc(1 * sizeof(t_rlist));
	if (!newelem)
		return ((t_rlist *) 0);
	newelem->n = n;
	newelem->next = newelem;
	newelem->prev = newelem;
	ind = 0;
	while (ind < 32)
	{
		newelem->bin[ind] = 0;
		ind++;
	}
	return (newelem);
}

// Insertion of an element just before the element <lst>
// 		(if <lst> was alone, ie has <prev> pointing to itself,
// 		then the <next> and <prev> of the new element both point to <lst>)
void	ft_lstadd_before(t_rlist **lst, t_rlist *newelem)
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

// Removal of 1 element from the list, replacing the links to maintain the rest
// 		/!\ removed element is not freed but returned as a 1-element list
// (if list had only 2 elements, the <prev> and <next> of the last element
// 	will be replaced correctly so that they point to that last element)
t_rlist	*ft_lstdelone(t_rlist *lst)
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
	return (lst);
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

// Returns a mallocated array containing the numbers of linked list <rlist>
int	*ft_rlist_to_array(t_rlist *lst)
{
	int		size;
	int		ind;
	int		*res;
	t_rlist *current;

	size = ft_lstsize(lst, lst, 0);
	res = malloc(size * sizeof(int));
	if (!res)
		return (NULL);
	ind = 0;
	current = lst;
	while (ind < size)
	{
		res[ind] = current->n;
		current = current->next;
		ind++;
	}
	return (res);
}
