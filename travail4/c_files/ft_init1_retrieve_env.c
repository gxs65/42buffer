#include "../h_files/ft_minishell.h"

// Searches the linked list (recursively) and returns the first element
// with the given <name>
t_var	*ft_var_find_by_name(t_var *current, char *name)
{
	if (!current)
		return (NULL);
	if (ft_strcmp(current->name, name) == 0)
		return (current);
	else
		return (ft_var_find_by_name(current->next, name));
}

// Searches the linked list (recursively) and deletes the first element
// with the given <name>,
// 		maintaining list coherence by returning the <next> element
// 		after the deleted one -> should be called this way :
// 			first_elem = ft_var_remove_by_name(first_elem, "str");
t_var	*ft_var_remove_by_name(t_var *current, char *name)
{
	t_var	*next;

	if (!current)
		return (NULL);
	if (ft_strcmp(current->name, name) == 0)
	{
		next = current->next;
		ft_var_delone(current);
		return (next);
	}
	else
	{
		current->next = ft_var_remove_by_name(current->next, name);
		return (current);
	}
}

// Stores all environment variables given in string array <envp>
// in a linked list whose reference to starting element is in <data.env>
// 		(all variables have flag <is_in_env> set to 1,
// 		 variables added later by the user will have this flag to 0
// 		 until the user exports the variable with builtin `export`)
int	ft_init1_retrieve_env(t_data *data, char **envp)
{
	int		ind_var;
	int		ind_equals;
	t_var	*var_start;
	t_var	*var_new;

	ind_var = 0;
	var_start = NULL;
	while (envp[ind_var])
	{
		ind_equals = ft_strchrp(envp[ind_var], '=');
		if (ind_equals == -1)
		{
			ft_var_list_clear(var_start);
			return (KILL_ENVFORMAT_ERROR);
		}
		envp[ind_var][ind_equals] = '\0';
		var_new = ft_var_new(envp[ind_var], envp[ind_var] + ind_equals + 1, 1);
		if (!var_new)
		{
			ft_var_list_clear(var_start);
			return (KILL_MALLOC_ERROR);
		}
		var_start = ft_var_push_back(var_start, var_new);
		ind_var++;
	}
	data->env = var_start;
	return (0);
}