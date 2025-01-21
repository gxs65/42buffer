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
	node->child_1 = NULL;
	node->child_2 = NULL;
	node->alloc_step = 0;
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
	node->fd_in_orig = -1;
	node->fd_out_orig = -1;
	node->depth = 0;
	return (node);
}

// Checks if tokens classified as a node's redir tokens are coherent :
// they come by two, with first a redirection meta-character, then a mere word
int	ft_check_redir_tokens_coherence(t_data *data, t_node *current)
{
	int	ind;
	int	token_type1;
	int	token_type2;

	if (current->nb_redir_tokens % 2 == 1)
		return (1);
	ind = 0;
	while (ind < current->nb_redir_tokens)
	{
		token_type1 = data->tokens->type[current->redir_tokens_inds[ind]];
		token_type2 = data->tokens->type[current->redir_tokens_inds[ind + 1]];
		if (!(ft_is_redir_token(token_type1) && token_type2 == CMD))
			return (1);
		ind += 2;
	}
	return (0);
}
