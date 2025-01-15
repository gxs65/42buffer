#include "../h_files/ft_minishell.h"

// Returns 1 if the tokens is a redirection meta-character
// ('<' '>' '<<' '>>')
int	ft_is_redir_token(int token_type)
{
	if (token_type == INPUT
		|| token_type == HEREDOC
		|| token_type == TRUNC
		|| token_type == APPEND)
		return (1);
	else
		return (0);
}

// Creates a new <t_node> node of a command tree with <malloc>,
// and initializes all its variables to
// 		- NULL, for arrrays to be allocated
// 		- (0), for allocated array sizes to be determined
// 		- (-1), for indexes to be determined
// 		- (0) or (1) for base stdin/stdout
t_node	*ft_node_new(void)
{
	t_node	*node;

	node = malloc(1 * sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = -1;
	node->back = NULL;
	node->child_1 = NULL;
	node->child_2 = NULL;
	node->cmd_tokens_inds = NULL;
	node->nb_cmd_tokens = 0;
	node->redir_tokens_inds = NULL;
	node->nb_redir_tokens = 0;
	node->cmd_words = NULL;
	node->nb_cmd_words = 0;
	node->redir_words = NULL;
	node->nb_redir_words = 0;
	node->ind_start_token = -1;
	node->ind_end_token = -1;
	node->fd_in = 0;
	node->fd_out = 1;
	return (node);
}
