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

// Determines if the node has an "=" token for shell variable assignation,
// and in this case if there is no syntax error in the assignation
// 		-> returns	(0) when no "="
// 					(-1) when "=" with syntax error
//					(1) when "=" with correct syntax
// Conditiens checked :
// 		- if the node has 0, 1 or >3 tokens, it can't be an assign node,
// 			so syntax is correct of no token is "="
// 		- if the node has 2 or 3 tokens, it is an assign node
// 			when the second token is "=", and if so the first
// 			(and second, if there is one) must be words ("cmd")
int	ft_parse2_has_correct_assign(t_data *data, t_node *current)
{
	int	ind;
	int	nb_tokens;

	nb_tokens = current->ind_end_token - current->ind_start_token + 1;
	if (nb_tokens != 3 && nb_tokens != 2)
		return (ft_parse2_has_no_equal(data, current));
	if (data->tokens->type[current->ind_start_token + 1] != EQUAL)
		return (0);
	else if ((nb_tokens == 2
		|| data->tokens->type[current->ind_start_token + 2] == CMD)
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
	return (0);
}