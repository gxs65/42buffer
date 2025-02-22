#include "../../h_files/ft_minirt.h"

t_object	*ft_object_new()
{
	t_object	*res;

	res = malloc(1 * sizeof(t_object));
	if (!res)
		return (NULL);
	return (res);
}

void	ft_object_push_back(t_object **start, t_object *current)
{
	if (*start)
		current->next = *start;
	else
		current->next = NULL;
	*start = current;
}