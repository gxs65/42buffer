#include "../h_files/ft_minishell.h"

// Applies recursion on from 1 node to its 2 children :
// each child receives the part of the chunk parent's chunk
// that is left/right of the meta-character at <ind>
int	ft_parse2_recurr_logical_pipe(t_data *data, t_node *current, int ind)
{
	int	start;
	int	end;

	start = current->ind_start_token;
	end = current->ind_end_token;
	ft_printf(LOGSV, "\tFound connector '%s' at %d\n",
		data->tokens->name[ind], ind);
	current->type = data->tokens->type[ind];
	current->child_1 = ft_node_new();
	current->child_2 = ft_node_new();
	if (!(current->child_1) || !(current->child_2))
		return (1);
	if (ft_parse2_build_tree_recurr(data, current->child_1, start, ind - 1))
		return (1);
	if (ft_parse2_build_tree_recurr(data, current->child_2, ind + 1, end))
		return (1);
	return (0);
}

// Applies recursion on from 1 node to its 1 child,
// used when the meta-character found is a parenthesis :
// the child receives the tokens enclosed within the parentheses
// 		(all tokens outside parentheses are considered redirection tokens)
int	ft_parse2_recurr_par(t_data *data, t_node *current, int opar, int cpar)
{
	ft_printf(LOGSV, "\tFound closing parenthesis at %d\n", cpar);
	current->type = OPEN;
	if (ft_parse2_tokens_ind_par(data, current, opar, cpar))
		return (1);
	current->child_1 = ft_node_new();
	if (!(current->child_1))
		return (1);
	return (ft_parse2_build_tree_recurr(data,
			current->child_1, opar + 1, cpar - 1));
}

// Ends recursion at a leaf node :
// retrieves and stores the indexes of its command and redir tokens,
// then returns 0
int	ft_parse2_endrecurr_leaf(t_data *data, t_node *current)
{
	ft_printf(LOGSV, "\tUnable to find parentheses\n");
	current->type = CMD;
	if (ft_parse2_tokens_ind_leaf(data, current))
		return (1);
	ft_printf(LOGS, "\t-> is a LEAF node, nb_cmd_tokens = %d\n",
		current->nb_cmd_tokens);
	return (0);
}

// Counts the number of tokens found by parse1 in the command line,
// creates the first node of the command tree <data.root>,
// then launches the recursive partitioning of the line
int	ft_parse2_build_tree(t_data *data)
{
	ft_printf(LOGS, "[PARSE2] Beginning tree build\n");
	data->nb_tokens = 0;
	while (data->tokens->type[data->nb_tokens] != -42)
		(data->nb_tokens)++;
	ft_printf(LOGS, "[PARSE2] Found %d tokens in line\n", data->nb_tokens);
	data->root = ft_node_new();
	ft_parse2_build_tree_recurr(data, data->root, 0, data->nb_tokens - 1);
	return (0);
}

// Recursive function receiving :
// 		- a chunk of the command line to analyse,
// 			from token at <current> to token at <end> (included)
// 		- the <current> node of the command tree,
// 			in which to store the results of analysis
// Operates by assigning a type to the <current node> :
// 		1. searching the chunk for meta-characters in priority order,
// 			so from less to most binding (logical < pipe < parentheses)
//		2. if there is a separating meta-character (type = OR/AND/PIPE/PAR),
// 			recurring on the 2 chunks separated by that characters
// 				(creating 2 children nodes, or 1 in the case of parentheses)
// 		3. if there is no separating meta-character (type = LEAF),
// 			stopping the recursion and retrieving the indexes of its tokens
// 				(with a distinction between command and redir tokens)
// /!\ search for logical meta-characters (OR/AND)
// 	   begins at end of command line to respect left associativity
// 			(<=> a meta-char on the left is more binding than on the right)
// /!\ in reality, there is also a retrieving of token indexes for PAR nodes,
// 		where all tokens are outside the parentheses and must be redir tokens
int	ft_parse2_build_tree_recurr(t_data *data, t_node *current,
	int start, int end)
{
	data->current = current;
	current->ind_start_token = start;
	current->ind_end_token = end;
	ft_printf(LOGS, "[PARSE2] On node from %d '%s' to %d '%s'\n",
		start, data->tokens->name[start],
		end, data->tokens->name[end]);
	return (ft_parse2_search_logical(data, current));
}
