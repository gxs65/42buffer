#include "../h_files/ft_minishell.h"

// Checks that pathname expansion on each redir word
// produces only one word
// 		- <ret> >=0 but !=1 is an "ambiguous redirection" syntax error
// 		- return value of <ft_parse_count_pathname> must be checked
// 			because of potential <opendir>/<readdir> errors
int	ft_parse3_count_redir_words_pathname(t_data *data, t_node *current)
{
	int		ind_word;
	int		ret;

	ind_word = 0;
	while (ind_word < current->nb_redir_words)
	{
		ret = ft_parseX_count_pathname(data, current->redir_words[ind_word]);
		if (ret  == -1)
			return (1);
		else if (ret != 1)
		{
			ft_printf(LOGS, "! ERROR : 0 or >1 redir words after pathname expansion\n");
			return (1);
		}
		ind_word++;
	}
	return (0);
}

// Stores redir words created by pathname expansion into array <res>
// (that must have been allocated with the adequate size)
// 		/!\ array <res> is given as argument to <ft_parseX_store_pathname>,
// 			which will fill it directly with its input
int	ft_parse3_store_redir_words_pathname(t_data *data,
	t_node *current, char **res)
{
	int		ind_oldword_in_node;
	int		ind_newword_in_node;

	ind_oldword_in_node = 0;
	ind_newword_in_node = 0;
	while (ind_oldword_in_node < current->nb_redir_words)
	{
		ft_printf(LOGSV, "         \t\tPathname expansion on word %s\n",
			current->redir_words[ind_oldword_in_node]);
		if (ft_parseX_store_pathname(data, current->redir_words[ind_oldword_in_node],
			res, &ind_newword_in_node))
			return (1);
		ind_oldword_in_node++;
	}
	return (0);
}

// Checks that pathname expansion on redir words yields exactly one word,
// allocates the <node.redir_words> array with the adequate size,
// and stores the expanded redir words in it
int	ft_parse3_expand_pathname_redir(t_data *data, t_node *current)
{
	char	**swapper_redir_words;

	ft_printf(LOGSV, "         \tc) pathname exp redir words\n");
	if (ft_parse3_count_redir_words_pathname(data, current))
		return (1);
	ft_printf(LOGSV, "         \tFound %d words after pathname expansion\n",
		current->nb_redir_words);
	swapper_redir_words = malloc((current->nb_redir_words + 1) * sizeof(char *));
	if (!swapper_redir_words)
		return (1);
	swapper_redir_words[current->nb_redir_words] = NULL;
	if (ft_parse3_store_redir_words_pathname(data, current, swapper_redir_words))
		return (1);
	free_strs_tab_upto(current->redir_words, current->nb_redir_words);
	current->redir_words = swapper_redir_words;
	return (0);
}
