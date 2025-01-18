#include "../h_files/ft_minishell.h"

// Retrieves indexes of all redirection tokens
// owned by parenthesis node <current> :
// 		they must be OUTSIDE the parentheses
// 	(<opar> = index of opening par, <cpar> = index of closing par)
// Checks if one of the tokens is a "=" meta-character,
// 		if yes it is a syntax error (parenthesis node can have no assignation)
int	ft_parse2_tokens_ind_par_retrieve(t_data *data, t_node *current,
	int opar, int cpar)
{
	int		ind_redir;
	int		ind;

	ind_redir = 0;
	ind = current->ind_start_token;
	while (ind < opar)
	{
		if (data->tokens->type[ind] == EQUAL)
			return (1);
		current->redir_tokens_inds[ind_redir] = ind;
		ind_redir++;
		ind++;
	}
	ind = cpar + 1;
	while (ind <= current->ind_end_token)
	{
		if (data->tokens->type[ind] == EQUAL)
			return (1);
		current->redir_tokens_inds[ind_redir] = ind;
		ind_redir++;
		ind++;
	}
	return (0);
}

// Counts if parenthesis node <current> has any redir tokens,
// 		if yes, retrieves their indexes in <node.redir_tokens_inds>
// (<ft_parse2_tokens_ind_par_retrieve> error case
//  is if it encounters a "=" token)
int	ft_parse2_tokens_ind_par(t_data *data, t_node *current,
	int opar, int cpar)
{
	ft_printf(LOGSV, "[PARSE2] \tReading redir tokens of parenthesis node\n");
	ft_printf(LOGSV, "         \tIndexes : start %d opar %d cpar %d end %d\n",
		current->ind_start_token, opar, cpar, current->ind_end_token);
	current->nb_redir_tokens = (opar - current->ind_start_token)
		+ (current->ind_end_token - cpar);
	current->nb_cmd_tokens = 0;
	ft_printf(LOGSV, "         \tCounted %d redir tokens\n",
		current->nb_redir_tokens);
	if (current->nb_redir_tokens == 0)
		return (0);
	current->redir_tokens_inds
		= malloc(current->nb_redir_tokens * sizeof(int));
	if (!(current->redir_tokens_inds))
		return (1);
	if (ft_parse2_tokens_ind_par_retrieve(data, current, opar, cpar))
	{
		free(current->redir_tokens_inds);
		return (STOP_ESYNTAX_VAR_ASSIGN);
	}
	return (0);
}

// Counts how many tokens of each type ('command' or 'redir')
// are owned by a node
// by going through the tokens and searching for redir meta-characters
void	ft_parse2_tokens_ind_cmd_count(t_data *data, t_node *current)
{
	int	ind;

	current->nb_redir_tokens = 0;
	ind = current->ind_start_token;
	while (ind <= current->ind_end_token)
	{
		if (ft_is_redir_token(data->tokens->type[ind]))
		{
			(current->nb_redir_tokens) += 2;
			ind++;
		}
		ind++;
	}
	current->nb_cmd_tokens = current->ind_end_token + 1
		- current->ind_start_token - current->nb_redir_tokens;
	ft_printf(LOGSV, "         \tCounted %d redir, %d cmd tokens\n",
		current->nb_redir_tokens, current->nb_cmd_tokens);
}

// Stores the indexes of the command and redir tokens owned by a node
// in the separate arrays <node.redir_tokens_inds> and <node.cmd_tokens_inds>
void	ft_parse2_tokens_ind_cmd_retrieve(t_data *data, t_node *current)
{
	int		ind;
	int		ind_cmd;
	int		ind_redir;

	ind = current->ind_start_token;
	ind_cmd = 0;
	ind_redir = 0;
	while (ind <= current->ind_end_token)
	{
		if (ft_is_redir_token(data->tokens->type[ind]))
		{
			current->redir_tokens_inds[ind_redir] = ind;
			current->redir_tokens_inds[ind_redir + 1] = ind + 1;
			ind_redir += 2;
			ind++;
		}
		else
		{
			current->cmd_tokens_inds[ind_cmd] = ind;
			ind_cmd++;
		}
		ind++;
	}
}

// Counts the number of command and redir tokens owned by a node,
// allocates an array to store their indexes, and stores their indexes
int	ft_parse2_tokens_ind_cmd(t_data *data, t_node *current)
{
	ft_printf(LOGSV, "[PARSE2] \tReading redir tokens of leaf cmd node\n");
	ft_parse2_tokens_ind_cmd_count(data, current);
	if (current->nb_cmd_tokens > 0)
		current->cmd_tokens_inds
			= malloc(current->nb_cmd_tokens * sizeof(int));
	if (current->nb_redir_tokens > 0)
		current->redir_tokens_inds
			= malloc(current->nb_redir_tokens * sizeof(int));
	ft_parse2_tokens_ind_cmd_retrieve(data, current);
	return (0);
}
