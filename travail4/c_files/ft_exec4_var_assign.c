#include "../h_files/ft_minishell.h"

// Updates the value of an existing variable
int	ft_exec4_var_existing(t_data *data, t_node *current,
	t_var *var, char *value)
{
	ft_printf(LOGSV, "[EXEC4] Updating variable %s with value %s\n",
		var->name, value);
	free(var->value);
	if (current->nb_cmd_words == 1)
		var->value = ft_strdup("");
	else
		var->value = ft_strdup(current->cmd_words[1]);
	if (!var->value)
		return (1);
	return (0);
}

// Creates a new variable with the given value,
// and pushes it at the beginning of the list of variables
int	ft_exec4_var_new(t_data *data, t_node *current, char *value)
{
	t_var	*var;

	ft_printf(LOGSV, "[EXEC4] New variable named %s with value %s\n",
		current->cmd_words[0], value);
	if (current->nb_cmd_words == 1)
		var = ft_var_new(current->cmd_words[0], "", 0);
	else
		var = ft_var_new(current->cmd_words[0], current->cmd_words[1], 0);
	if (!var)
		return (1);
	data->env = ft_var_push_back(data->env, var);
	return (0);
}

// Executes a "EQUAL" node <=> assigns a shell variable,
// whose name and value are stored in <current.cmd_words>
// 		\ if the node has 1 cmd word, the value is ""
// 		\ if the node has 2 cmd words, the value is <cmd_words[1]>
// 		\ if the node has >2 cmd words,
// 			the value is the result of a join on all words after the first
// Depending on wether the variable name is found among existing variables,
// updates the value of an existing variable, or creates a new one
int	ft_exec4_var_assign(t_data *data, t_node *current)
{
	t_var	*var;
	char	*value;

	if (current->nb_cmd_words == 1)
		value = ft_strdup("");
	else if (current->nb_cmd_words == 2)
		value = ft_strdup(current->cmd_words[1]);
	else
		value = ft_strjoin_str_array(current->nb_cmd_words - 1,
			current->cmd_words + 1, " ");
	if (!value)
		return (1);
	var = ft_var_find_by_name(data->env, current->cmd_words[0]);
	if (var && ft_exec4_var_existing(data, current, var, value))
	{
		free(value);
		return (1);
	}
	else if (ft_exec4_var_new(data, current, value))
	{
		free(value);
		return (1);
	}
	free(value);
	return (0);
}