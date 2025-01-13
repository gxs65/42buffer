#include "../h_files/ft_minishell.h"

// Performs operations modifying the token strings,
// required by the last meta-characters not yet handled : '$' and '*'
// 		- shell variables expansion ('$')
// 		- pathname expansion ('*')
// 		- splitting each non-quoted token by space characters
// 		- quote removal
int	ft_parse3_expand(t_data *data)
{
	ft_printf(LOGS, "[PARSE3] Beginning token expansion");
	if (ft_parse3_expand_shellvar(data))
		return (1);
	return (0);
}

// Goes through the array of tokens and applies shellvar expansion to each
// (no need to recursively go through the command tree,
// 		because shellvar expansion on a token does not require to know
// 		the type of the node that owns the token)
int	ft_parse3_expand_shellvar(t_data *data)
{
	int	ind;
	int	len;

	ind = 0;
	while (ind < data->nb_tokens)
	{
		ft_printf(LOGSV, "[PARSE3] Shellvar expansion on token %s\n",
			data->tokens->name[ind]);
		len = ft_strlen(data->tokens->name[ind]);
		if (len > 0 && ft_parse3_expand_shellvar_on_tok(data, ind))
			return (1);
	}
	return (0);
}


/*
int	ft_parse3_expand_shellvar(t_data *data, t_node *current)
{
	int	ind;

	ft_printf(LOGSV, "[PARSE3] On node type %d, nb_cmd_tokens %d\n",
		current->type, current->nb_cmd_tokens);
	if (current->nb_cmd_tokens > 0)
	{
		ft_printf(LOGSV, "\tStarting expansion on cmd tokens\n");
		ind = 0;
		while (ind < current->nb_cmd_tokens)
		{
			ft_parse3_expand_shellvar(data, current->cmd_tokens_inds[ind]);
			ind++;
		}
	}
	if (current->child_1)
		ft_parse3_expand_recurr(data, current->child_1);
	if (current->child_2)
		ft_parse3_expand_recurr(data, current->child_2);
	return (0);
}
*/
