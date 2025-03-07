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

// Checks if there exists a shell variables with name "PATH"
// (not necessarily a variable belonging to env :
//  bash takes into account a PATH even if it was not exported)
// and splits it into directories, to allow later search for binaries
int	ft_init1_det_path(t_data *data)
{
	t_var	*path_var;

	if (data->path_dirs)
		free_strs_tab_nullterm(data->path_dirs);
	data->path_dirs = NULL;
	path_var = ft_var_find_by_name(data->env, "PATH");
	if (!path_var)
		data->path_dirs = ft_split("", ':');
	else
		data->path_dirs = ft_split(path_var->value, ':');
	if (!(data->path_dirs))
		return (1);
	return (0);
}

int	ft_init1_retrieve_env(t_data *data, char **envp)
{
	int		ind_var;
	int		ind_equals;
	t_var	*var_start;
	t_var	*var_new;

	ind_var = 0;
	while (envp[ind_var])
	{
		ind_equals = ft_strchrp(envp[ind_var], '=');
		if (ind_equals == -1)
		{
			ft_var_list_clear(var_start);
			return (1);
		}
		envp[ind_var][ind_equals] = '\0';
		var_new = ft_var_new(envp[ind_var], envp[ind_var] + ind_equals + 1, 1);
		if (!var_new)
		{
			ft_var_list_clear(var_start);
			return (1);
		}
		var_start = ft_var_push_back(var_start, var_new);
		ind_var++;
	}
	data->env = var_start;
	return (0);
}