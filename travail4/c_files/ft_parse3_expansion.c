#include "../h_files/ft_minishell.h"

// Performs operations modifying the token strings,
// required by the last meta-characters not yet handled : '$' and '*'
// 		- shell variables expansion ('$')
// 		- pathname expansion ('*')
// 		- splitting each non-quoted token by space characters
// 		- quote removal
int	ft_parse3_expand(t_data *data)
{
	ft_printf(LOGS, "[PARSE3] Beginning parse3 = \
		a) shellvar exp, b) resplit, c) pathname exp, d) quote removal\n");
	if (ft_parse3_stepA_expand_shellvar(data))
		return (1);
	if (ft_parse3_stepsBCD_recurr(data, data->root))
		return (1);
	return (0);
}

// Goes through the array of tokens and applies shellvar expansion to each
// (no need to recursively go through the nodes of the command tree,
// 		because shellvar expansion on a token does not require to know
// 		the type of the node that owns the token)
// A token may be expanded if it is not a meta-character,
// and it has length more than 1 (a single '$' would remain so)
// 		/!\ skipping quoted '$' is performed by <ft_parse3_expand_sv_on_tok>
int	ft_parse3_stepA_expand_shellvar(t_data *data)
{
	int	pos_tok;

	pos_tok = 0;
	while (pos_tok < data->nb_tokens)
	{
		ft_printf(LOGSV, "[PARSE3] a) Shellvar expansion on token %s\n",
			data->tokens->name[pos_tok]);
		if (data->tokens->type[pos_tok] == CMD
			&& ft_strlen(data->tokens->name[pos_tok]) > 1
			&& ft_parse3_expand_sv_on_tok(data, pos_tok))
			return (1);
		pos_tok++;
	}
	return (0);
}

// Recursively goes through the nodes of the command tree to apply the steps :
// 		- split command tokens into command words, by (non-quoted) whitespace
// 		- expand the wildcards in command words (= pathname expansion)
// 		- try to split the redir tokens by whitespace :
// 				error if produces anything other than exactly 1 word
// 		- try to expand the wildcards in redir words :
// 				error if produces anything other than exactly 1 word
// (only CMD nodes may have command tokens,
//  CMD and PARENTHESIS nodes both may have redir tokens)
int	ft_parse3_stepsBCD_recurr(t_data *data, t_node *current)
{
	ft_printf(LOGSV, "[PARSE3] b-c-d) recurr, node type %d\n", current->type);
	if (current->type == CMD || current->type == EQUAL)
	{
		if (ft_parse3_resplit_cmd_tokens(data, current))
			return (1);
		if (ft_parse3_expand_pathname_cmd(data, current))
			return (1);
		ft_parse3_remove_quotes_cmd(data, current);
	}
	if (current->type == CMD || current->type == EQUAL
		|| current->type == OPEN)
	{
		if (ft_parse3_resplit_redir_tokens(data, current))
			return (1);
		if (ft_parse3_expand_pathname_redir(data, current))
			return (1);
		ft_parse3_remove_quotes_redir(data, current);
	}

	if (current->child_1 && ft_parse3_stepsBCD_recurr(data, current->child_1))
		return (1);
	if (current->child_2 && ft_parse3_stepsBCD_recurr(data, current->child_2))
		return (1);
	return (0);
}
