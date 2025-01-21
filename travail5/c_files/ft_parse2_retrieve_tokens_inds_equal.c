#include "../h_files/ft_minishell.h"

// Returns -1 if the current node has a "=" meta-character among its tokens,
// 0 otherwise
int	ft_parse2_has_no_equal(t_data *data, t_node *current)
{
	int	ind;

	ind = current->ind_start_token;
	while (ind <= current->ind_end_token)
	{
		if (data->tokens->type[ind] == EQUAL)
			return (-1);
		ind++;
	}
	return (0);
}

// Determines if the node has a syntax error related to "=" tokens
// (token for shell variable assignation)
// 		-> returns	(0)  when no "="
// 					(-1) when "=" when syntax error
//					(1)  when "=" with correct syntax
// Conditions checked :
// 		- if the node has 0 or 1 or >3 tokens (<=> it can't be an assign node)
// 			and it has no token "=", syntax is correct
// 		- else if the node has 2 or 3 tokens
// 			and none is "=", syntax is correct
// 		- else if the node has 2 or 3 tokens,
// 			and the 2nd is "=" and others are "cmd", syntax is correct
// 		- in any other case, there is a syntax error
int	ft_parse2_has_correct_assign(t_data *data, t_node *current)
{
	int	ind;
	int	nb_tokens;

	nb_tokens = current->ind_end_token - current->ind_start_token + 1;
	if (nb_tokens == 0)
		return (-1);
	if (nb_tokens != 3 && nb_tokens != 2)
		return (ft_parse2_has_no_equal(data, current));
	if ((nb_tokens == 2 ||
		data->tokens->type[current->ind_start_token + 2] != EQUAL)
		&& data->tokens->type[current->ind_start_token + 1] != EQUAL
		&& data->tokens->type[current->ind_start_token] != EQUAL)
		return (0);
	else if ((nb_tokens == 2
		|| data->tokens->type[current->ind_start_token + 2] == CMD)
		&& data->tokens->type[current->ind_start_token + 1] == EQUAL
		&& data->tokens->type[current->ind_start_token] == CMD)
		return (1);
	else
		return (-1);
}

// Stores the 1 or 2 indexes of the tokens of an assign node :
// first token (variable name) + third token if it exists (variable value)
int	ft_parse2_tokens_ind_equal(t_data *data, t_node *current)
{
	ft_printf(LOGSV, "[PARSE2] \tReading redir tokens of leaf equal node\n");
	current->nb_cmd_tokens = current->ind_end_token - current->ind_start_token;
	current->cmd_tokens_inds = malloc(sizeof(int) * current->nb_cmd_tokens);
	if (!current->cmd_tokens_inds)
		return (1);
	current->cmd_tokens_inds[0] = current->ind_start_token;
	if (current->nb_cmd_tokens == 2)
		current->cmd_tokens_inds[1] = current->ind_end_token;
	current->alloc_step = 1;
	return (0);
}