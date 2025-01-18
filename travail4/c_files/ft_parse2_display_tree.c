#include "../h_files/ft_minishell.h"

// LOG FILE, DOES NOT RESPECT THE NORM

// Displays the elements of <array> whose indexes
// are given in <inds> (<inds> has length <len>)
void	ft_parse2_display_part_array(char **array, int *inds, int len)
{
	int	ind;

	ind = 0;
	while (ind < len)
	{
		ft_printf(LOGS, "%s ", array[inds[ind]]);
		ind++;
	}
}

// Displays the tokens owned by a node :
// 		\ redir tokens for parenthesis nodes,
// 		\ redir tokens and cmd tokens for leaf nodes
void	ft_parse2_display_node_tokens(t_data *data, t_node *current)
{
	if ((current->type == CMD || current->type == OPEN ||
		current->type == EQUAL))
	{
		ft_printf(LOGS, "redir { ");
		ft_parse2_display_part_array(data->tokens->name,
			current->redir_tokens_inds, current->nb_redir_tokens);
		if (current->type == CMD || current->type == EQUAL)
		{
			ft_printf(LOGS, "}; cmd { ");
			ft_parse2_display_part_array(data->tokens->name,
				current->cmd_tokens_inds, current->nb_cmd_tokens);
		}
		ft_printf(LOGS, "}\n");
	}
	else
		ft_printf(LOGS, "\n");
}

// Displays the command tree in its state between parse2 and parse3
// (using the arrays <node.redir_tokens_ind> and <node.cmd_tokens_ind>)
void	ft_parse2_display_cmd_tree(t_data *data, t_node *current, int depth)
{
	int		ind;
	char	*descriptors[] = {"(null)", "input", "heredoc",
							"trunc", "append", "leaf",
							"pipe", "and", "or", "par", "par", "equal"};

	if (depth == 0)
		ft_printf(LOGS, "##### COMMAND TREE PARSE2 #####\n");
	ind = 0;
	while (ind < depth)
	{
		ft_printf(LOGS, "\t");
		ind++;
	}
	ft_printf(LOGS, "%s : ", descriptors[current->type]);
	ft_parse2_display_node_tokens(data, current);
	if (current->child_1)
		ft_parse2_display_cmd_tree(data, current->child_1, depth + 1);
	if (current->child_2)
		ft_parse2_display_cmd_tree(data, current->child_2, depth + 1);
	if (depth == 0)
		ft_printf(LOGS, "##### ------------------- #####\n");
}
