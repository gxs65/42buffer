#include "../h_files/ft_minishell.h"

// Creates a new <var> element : allocates first the element itself,
// 		then the strings it contains
t_var	*ft_var_new(char *name, char *value, int is_in_env)
{
	t_var	*var;

	ft_printf(LOGSV, "[INIT1] Creating var element for name %s, value %s\n",
		name, value);
	var = malloc(1 * sizeof(t_var));
	if (!var)
		return (NULL);
	var->value = ft_strdup(value);
	if (!var->value)
	{
		free(var);
		return (NULL);
	}
	var->name = ft_strdup(name);
	if (!var->name)
	{
		free(var->value);
		free(var->name);
		return (NULL);
	}
	var->is_in_env = is_in_env;
	var->next = NULL;
	return (var);
}

// Adds a new <var> element at the beginning of the list,
// returns the new start element of the list -> should be called this way :
// 		first_elem = ft_var_push_back(first_elem, new_elem);
t_var	*ft_var_push_back(t_var *start, t_var *new_var)
{
	new_var->next = start;
	return (new_var);
}

// Frees and deletes one <var> element
void	ft_var_delone(t_var *var)
{
	free(var->name);
	free(var->value);
	free(var);
}

// Frees and deletes all <var> list elements (recursively) 
void	ft_var_list_clear(t_var *current)
{
	if (current)
	{
		ft_var_list_clear(current->next);
		ft_var_delone(current);
	}
}