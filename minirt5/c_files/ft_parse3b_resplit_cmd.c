#include "../h_files/ft_minishell.h"

// Counts the command words that the node will have
// after command tokens are splitted by whitespace
void	ft_parse3_count_cmd_words_resplit(t_data *data, t_node *current)
{
	int		ind_token_in_node;

	ind_token_in_node = 0;
	current->nb_cmd_words = 0;
	while (ind_token_in_node < current->nb_cmd_tokens)
	{
		current->nb_cmd_words += ft_count_tokens_space(
			data->tokens->name[current->cmd_tokens_inds[ind_token_in_node]]);
		ind_token_in_node++;
	}
	ft_printf(LOGSV, "         \tFound %d words after resplit\n", current->nb_cmd_words);
}

// Stores the command words splitted by whitespace
// in the newly allocated array <current.cmd_words>
int	ft_parse3_store_cmd_words_resplit(t_data *data, t_node *current)
{
	int		ind_token_in_node;
	int		ind_word_in_node;
	int		ind_word_in_token;
	char	**words;

	ind_word_in_node = 0;
	ind_token_in_node = 0;
	while (ind_token_in_node < current->nb_cmd_tokens)
	{
		words = ft_split_space(data->tokens->name[
			current->cmd_tokens_inds[ind_token_in_node]]);
		if (!words)
			return (1);
		ind_word_in_token = 0;
		while (words[ind_word_in_token])
		{
			current->cmd_words[ind_word_in_node] = words[ind_word_in_token];
			ft_printf(LOGSV, "         \t\tAdded word : %s\n", current->cmd_words[ind_word_in_node]);
			ind_word_in_node++;
			ind_word_in_token++;
		}
		free(words);
		ind_token_in_node++;
	}
	return (0);
}
// TODO : transform to act like <store_pathname> : give <current.cmd_words>
// as argument to the splitter so that it fills it with th  found tokens
// 		(the previous count ensures enough space allocated,
// 		 and it avoids the <malloc> of one <char **>)

// Counts the command words after split by whitespace,
// allocates a new string array <current.cmd_words> of that size,
// and stores the command words
int	ft_parse3_resplit_cmd_tokens(t_data *data, t_node *current)
{
	ft_printf(LOGSV, "         \tb) resplit command words\n");
	ft_parse3_count_cmd_words_resplit(data, current);
	current->cmd_words = malloc((current->nb_cmd_words + 1) * sizeof(char *));
	if (!(current->cmd_words))
		return (1);
	current->cmd_words[current->nb_cmd_words] = NULL;
	if (ft_parse3_store_cmd_words_resplit(data, current))
		return (1);
	return (0);
}
