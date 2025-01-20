#include "../h_files/ft_minishell.h"

// Performs operations modifying the token strings,
// required by the last meta-characters not yet handled : '$' and '*'
// 		- shell variables expansion ('$')
// 		- pathname expansion ('*')
// 		- splitting each non-quoted token by space characters
// 		- quote removal
int	ft_parse3_expand(t_data *data)
{
	int	ret;

	ft_printf(LOGS, "[PARSE3] Beginning parse3 = \
		a) shellvar exp, b) resplit, c) pathname exp, d) quote removal\n");
	ret = ft_parse3_stepA_expand_shellvar(data);
	if (ret)
		return (ret);
	ret = ft_parse3_stepsBCD_recurr(data, data->root);
	if (ret)
		return (ret);
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
			return (KILL_MALLOC_ERROR);
		pos_tok++;
	}
	return (0);
}

// Performs steps B (replit) C (wildcard expansion) D (quote removal)
// on command and redir tokens according to node type
// (error cases : 	only malloc for resplit on cmd tokens,
// 					split and expansion giving 0 or >tokens on redir tokens,
// 					opening directory for wildcard expansion)
int	ft_parse3_perform_stepsBCD(t_data *data, t_node *current)
{
	int	ret;

	if (current->type == CMD || current->type == EQUAL)
	{
		if (ft_parse3_resplit_cmd_tokens(data, current))
			return (KILL_MALLOC_ERROR);
		ret = ft_parse3_expand_pathname_cmd(data, current);
		if (ret)
			return (ret);
		ft_parse3_remove_quotes_cmd(data, current);
	}
	if (current->type == CMD || current->type == EQUAL
		|| current->type == OPEN)
	{
		ret = ft_parse3_resplit_redir_tokens(data, current);
		if (ret)
			return (ret);
		ret = ft_parse3_expand_pathname_redir(data, current);
		if (ret)
			return (ret);
		ft_parse3_remove_quotes_redir(data, current);
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
	int	ret;

	ft_printf(LOGSV, "[PARSE3] b-c-d) recurr, node type %d\n", current->type);
	ret = ft_parse3_perform_stepsBCD(data, current);
	if (ret)
		return (ret);
	if (current->child_1)
	{
		ret = ft_parse3_stepsBCD_recurr(data, current->child_1);
		if (ret)
			return (ret);
	}
	if (current->child_2)
	{
		ret = ft_parse3_stepsBCD_recurr(data, current->child_2);
		if (ret)
			return (ret);
	}
	return (0);
}
