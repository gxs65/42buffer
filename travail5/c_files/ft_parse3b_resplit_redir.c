#include "../h_files/ft_minishell.h"

// Checks is the syntax is correct : the result of split by whitespace
// on a redir token (that is not a meta-character) must be exactly 1 word
// Returns 0 if all tokens respect that condition, and 1 if not
int	ft_parse3_count_redir_words_resplit(t_data *data, t_node *current)
{
	int		ind_token_in_node;
	int		nb_splitted;

	ind_token_in_node = 1;
	current->nb_redir_words = 0;
	while (ind_token_in_node < current->nb_redir_tokens)
	{
		nb_splitted = ft_count_tokens_space(
			data->tokens->name[current->redir_tokens_inds[ind_token_in_node]]);
		if (nb_splitted != 1)
			return (STOP_SYNTAX_ERROR);
		current->nb_redir_words++;
		ind_token_in_node += 2;
	}
	ft_printf(LOGSV, "         \tFound %d redir words after resplit\n",
		current->nb_redir_words);
	return (0);
}

// Stores the redirection words splitted by whitespace
// (this split should only be a strip for redir tokens)
// in the newly allocated array <current.redir_words>
int	ft_parse3_store_redir_words_resplit(t_data *data, t_node *current)
{
	int		ind_token_in_node;
	int		ind_word_in_node;
	char	**words;

	ind_token_in_node = 1;
	ind_word_in_node = 0;
	while (ind_token_in_node < current->nb_redir_tokens)
	{
		words = ft_split_space(data->tokens->name[
			current->redir_tokens_inds[ind_token_in_node]]);
		if (!words)
			return (1);
		current->redir_words[ind_word_in_node] = words[0];
		ft_printf(LOGSV, "         \t\tAdded word : %s\n",
			current->redir_words[ind_word_in_node]);
		free(words);
		ind_word_in_node++;
		ind_token_in_node += 2;
	}
	return (0);
}

// Counts the redirection words after split by whitespace :
// 		this number must be equal to nb_redir_tokens / 2,
// 			anything else is a syntax error
// then allocates a new string array <current.redir_words> of that size,
// and stores the redirection words
int	ft_parse3_resplit_redir_tokens(t_data *data, t_node *current)
{
	ft_printf(LOGSV, "         \tb) resplit redir words\n");
	if (ft_parse3_count_redir_words_resplit(data, current))
		return (STOP_SYNTAX_ERROR);
	current->redir_words = malloc(
		(current->nb_redir_words + 1) * sizeof(char *));
	if (!(current->redir_words))
		return (KILL_MALLOC_ERROR);
	current->redir_words[current->nb_redir_words] = NULL;
	if (ft_parse3_store_redir_words_resplit(data, current))
		return (KILL_MALLOC_ERROR);
	current->alloc_step = 3;
	return (0);
}
