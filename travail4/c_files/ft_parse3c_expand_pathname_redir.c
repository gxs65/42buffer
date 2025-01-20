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
		if (ft_strchrp_nonquoted(current->redir_words[ind_word], '*', 2, 0)
			!= -1)
		{
			ret = ft_parse3_count_pathname(current->redir_words[ind_word]);
			if (ret  == -1)
				return (KILL_OPENCWD_ERROR);
			else if (ret != 1)
				return (STOP_SYNTAX_ERROR);
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
	int		ind_word;
	int		ind_new;

	ind_word = 0;
	ind_new = 0;
	while (ind_word < current->nb_redir_words)
	{
		ft_printf(LOGSV, "         \t\tPathname expansion on word %s\n",
			current->redir_words[ind_word]);
		if (ft_strchrp_nonquoted(current->redir_words[ind_word], '*', 2, 0) == -1)
		{
			ft_printf(LOGSV, "         \t\t-> no wildcard detected\n");
			res[ind_new] = ft_strdup(current->redir_words[ind_word]);
			if (!res[ind_new])
				return (1);
			ind_new++;
		}
		else
		{
			if (ft_parse3_store_pathname(current->redir_words[ind_word],
				res, &ind_new))
				return (1);
		}
		ind_word++;
	}
	return (0);
}

// Checks that pathname expansion on redir words yields exactly one word,
// allocates the <node.redir_words> array with the adequate size,
// and stores the expanded redir words in it
int	ft_parse3_expand_pathname_redir(t_data *data, t_node *current)
{
	char	**swapper_redir_words;
	int		ret;

	ft_printf(LOGSV, "         \tc) pathname exp redir words\n");
	ret = ft_parse3_count_redir_words_pathname(data, current);
	if (ret)
		return (ret);
	ft_printf(LOGSV, "         \tno ambiguous expansion to 0 or >1 tokens\n",
		current->nb_redir_words);
	swapper_redir_words = malloc((current->nb_redir_words + 1) * sizeof(char *));
	if (!swapper_redir_words)
		return (KILL_MALLOC_ERROR);
	swapper_redir_words[current->nb_redir_words] = NULL;
	if (ft_parse3_store_redir_words_pathname(data, current, swapper_redir_words))
		return (KILL_MALLOC_ERROR);
	free_strs_tab_upto(current->redir_words, current->nb_redir_words);
	current->redir_words = swapper_redir_words;
	return (0);
}
