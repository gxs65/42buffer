#include "../h_files/ft_minishell.h"

// Goes through the tokens of node <current> to search
// for logical meta-characters ('||' or '&&'),
// recurrs when finding one / goes to pipe search when finding none
int	ft_parse2_search_logical(t_data *data, t_node *current)
{
	int	open_pars;
	int	ind;
	int	*types;

	ft_printf(LOGSV, "\tBeginning meta-character search\n");
	open_pars = 0;
	ind = current->ind_end_token;
	types = data->tokens->type;
	while (ind >= current->ind_start_token)
	{
		if (types[ind] == CLOSE)
			open_pars++;
		else if (types[ind] == OPEN)
			open_pars--;
		else if (open_pars == 0)
		{
			if (types[ind] == OR || types[ind] == AND)
				return (ft_parse2_recurr_logical_pipe(data, current, ind));
		}
		ind--;
	}
	return (ft_parse2_search_pipe(data, current));
}

// Goes through the tokens of node <current> to search for pipe meta-character
// recurrs when finding one / goes to parenthesis search when finding none
int	ft_parse2_search_pipe(t_data *data, t_node *current)
{
	int	open_pars;
	int	ind;
	int	*types;

	ft_printf(LOGSV, "\tUnable to find logical meta-character\n");
	open_pars = 0;
	ind = current->ind_end_token;
	types = data->tokens->type;
	while (ind >= current->ind_start_token)
	{
		if (types[ind] == CLOSE)
			open_pars++;
		else if (types[ind] == OPEN)
			open_pars--;
		else if (open_pars == 0)
		{
			if (types[ind] == PIPE)
				return (ft_parse2_recurr_logical_pipe(data, current, ind));
		}
		ind--;
	}
	return (ft_parse2_search_par(data, current));
}

// Goes through the tokens of node <current> to search for parentheses
// recurrs when finding the first pair of pars /
// goes to end of recursion when finding none (<=> the node is a leaf node)
int	ft_parse2_search_par(t_data *data, t_node *current)
{
	int	ind;
	int	open_pars;
	int	ind_first_open_par;

	ft_printf(LOGSV, "\tUnable to find pipe\n");
	open_pars = 0;
	ind = current->ind_start_token;
	while (ind <= current->ind_end_token)
	{
		if (data->tokens->type[ind] == OPEN)
		{
			if (open_pars == 0)
				ind_first_open_par = ind;
			open_pars++;
		}
		else if (data->tokens->type[ind] == CLOSE)
		{
			if (open_pars == 1)
				return (ft_parse2_recurr_par(data, current,
						ind_first_open_par, ind));
			open_pars--;
		}
		ind++;
	}
	return (ft_parse2_endrecurr_leaf(data, current));
}
