#include "../h_files/ft_minishell.h"

// LOG FILE, DOES NOT RESPECT THE NORM

// Displays redirection words of a node,
// associated to their redirection meta-character
void	ft_parse3_display_redir_words(t_data *data, t_node *current)
{
	int	ind;

	if ((current->type == CMD || current->type == OPEN))
	{
		ft_printf(LOGS, "%d*redir { ", current->nb_redir_words);
		ind = 0;
		while (ind < current->nb_redir_words)
		{
			ft_printf(LOGS, "%s %s ",
				data->tokens->name[current->redir_tokens_inds[ind * 2]],
				current->redir_words[ind]);
			ind++;
		}
		ft_printf(LOGS, "} ");
	}
	else
		ft_printf(LOGS, " ");
}

// Displays command words in a node
void	ft_parse3_display_cmd_words(t_data *data, t_node *current)
{
	int	ind;

	if (current->type == CMD || current->type == EQUAL)
	{
		ft_printf(LOGS, "%d*cmd { ", current->nb_cmd_words);
		ft_display_string_array(current->cmd_words,
			current->nb_cmd_words);
		ft_printf(LOGS, "}\n");
	}
	else
		ft_printf(LOGS, "\n");
}

// Displays the command tree in its state between after parse3
// (using the arrays <node.cmd_words> and <node.redir_words>)
void	ft_parse3_display_cmd_tree(t_data *data, t_node *current, int depth)
{
	int		ind;
	char	*descriptors[] = {"(null)", "input", "heredoc",
							"trunc", "append", "leaf",
							"pipe", "and", "or", "par", "par", "equal"};

	if (depth == 0)
		ft_printf(LOGS, "##### COMMAND TREE PARSE3 #####\n");
	ind = 0;
	while (ind < depth)
	{
		ft_printf(LOGS, "\t");
		ind++;
	}
	ft_printf(LOGS, "%s : ", descriptors[current->type]);
	ft_parse3_display_redir_words(data, current);
	ft_parse3_display_cmd_words(data, current);
	if (current->child_1)
		ft_parse3_display_cmd_tree(data, current->child_1, depth + 1);
	if (current->child_2)
		ft_parse3_display_cmd_tree(data, current->child_2, depth + 1);
	if (depth == 0)
		ft_printf(LOGS, "##### ------------------- #####\n");
}
