#include "../h_files/ft_minishell.h"

// BLANK VERSIONS //
// /!\ should perform quote removal before analysis
int	ft_parseX_count_pathname(t_data *data, char *word)
{
	(void)data;
	(void)word;
	return (1);
}
/* ft_printf(LOGSV, "received word %s, index before incr = %d\n", word, *index_to_incr);
	ft_printf(LOGSV, "received store : { ");
	ft_display_string_array(store, *index_to_incr);
	ft_printf(LOGSV, "}\n"); */
int	ft_parseX_store_pathname(t_data *data, char *word, char **store,
	int *index_to_incr)
{
	(void)data;
	store[(*index_to_incr)] = ft_strdup(word);
	*index_to_incr += 1;
	return (0);
}
// BLANK VERSIONS //

// Counts (in variable <count> how many command words the current node
// shoud have after pathname expansion
// 		- the number of command words can only stagnate or increase,
// 			since a word with '*', but matching no file in CWD, is not changed
// 		- return value of <ft_parse_count_pathname> must be checked
// 			because of potential <opendir>/<readdir> errors
int	ft_parse3_count_cmd_words_pathname(t_data *data, t_node *current,
	int *count)
{
	int		ind_word;
	int		ret;

	ind_word = 0;
	*count = 0;
	while (ind_word < current->nb_cmd_words)
	{
		ret = ft_parseX_count_pathname(data, current->cmd_words[ind_word]);
		if (ret == -1)
			return (1);
		*count += ret;
		ind_word++;
	}
	ft_printf(LOGSV, "         \tFound %d words after pathname expansion\n",
		*count);
	return (0);
}

// Stores command words created by pathname expansion into array <res>
// (that must have been allocated with the adequate size)
// 		/!\ array <res> is given as argument to <ft_parseX_store_pathname>,
// 			which will fill it directly with its input
int	ft_parse3_store_cmd_words_pathname(t_data *data,
	t_node *current, char **res)
{
	int		ind_oldword_in_node;
	int		ind_newword_in_node;

	ind_oldword_in_node = 0;
	ind_newword_in_node = 0;
	while (ind_oldword_in_node < current->nb_cmd_words)
	{
		ft_printf(LOGSV, "         \t\tPathname expansion on word %s\n",
			current->cmd_words[ind_oldword_in_node]);
		if (ft_parseX_store_pathname(data, current->cmd_words[ind_oldword_in_node],
			res, &ind_newword_in_node))
			return (1);
		ind_oldword_in_node++;
	}
	return (0);
}

// Counts the command words after pathname expansion,
// allocates a new string array of that size, stores the command words in it,
// and replaces array <data.cmd_words> by this new array
int	ft_parse3_expand_pathname_cmd(t_data *data, t_node *current)
{
	char	**swapper_cmd_words;
	int		swapper_nb_cmd_words;

	ft_printf(LOGSV, "         \tc) pathname exp command words\n");
	if (ft_parse3_count_cmd_words_pathname(data, current, &swapper_nb_cmd_words))
		return (1);
	swapper_cmd_words = malloc((swapper_nb_cmd_words + 1) * sizeof(char *));
	if (!swapper_cmd_words)
		return (1);
	swapper_cmd_words[swapper_nb_cmd_words] = NULL;
	if (ft_parse3_store_cmd_words_pathname(data, current, swapper_cmd_words))
		return (1);
	free_strs_tab_upto(current->cmd_words, current->nb_cmd_words);
	current->cmd_words = swapper_cmd_words;
	current->nb_cmd_words = swapper_nb_cmd_words;
	return (0);
}
// not handled by this implementation :
// 		- words corresponding to files
// 		  expanded from pathname expansion are not marked as such,
// 		  so quote removal will be applied to them even if it should not
// 		- if the file structure is changed by adding/removing files in CWD
// 		  between execution of <ft_parseX_count_pathname> and
// 		  <ft_parseX_store_pathname>, the program may segfault