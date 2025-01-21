#include "../h_files/ft_minishell.h"

// Retrieves indexes of all redirection tokens
// owned by parenthesis node <current> :
// 		they must be OUTSIDE the parentheses
// 	(<opar> = index of opening par, <cpar> = index of closing par)
int	ft_parse2_tokens_ind_par_retrieve(t_data *data, t_node *current,
	int opar, int cpar)
{
	int		ind_redir;
	int		ind;

	ind_redir = 0;
	ind = current->ind_start_token;
	while (ind < opar)
	{
		current->redir_tokens_inds[ind_redir] = ind;
		ind_redir++;
		ind++;
	}
	ind = cpar + 1;
	while (ind <= current->ind_end_token)
	{
		current->redir_tokens_inds[ind_redir] = ind;
		ind_redir++;
		ind++;
	}
	return (0);
}

// Counts if parenthesis node <current> has any redir tokens,
// 		if yes, retrieves their indexes in <node.redir_tokens_inds>
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
	{
		current->alloc_step = 1;
		return (0);
	}
	current->redir_tokens_inds
		= malloc(current->nb_redir_tokens * sizeof(int));
	if (!(current->redir_tokens_inds))
		return (1);
	ft_parse2_tokens_ind_par_retrieve(data, current, opar, cpar);
	current->alloc_step = 1;
	return (0);
}