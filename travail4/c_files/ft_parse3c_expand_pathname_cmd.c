#include "../h_files/ft_minishell.h"

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
		if (ft_strchrp_nonquoted(current->cmd_words[ind_word], '*', 2, 0)
			== -1)
			*count += 1;
		else
		{
			ret = ft_parse3_count_pathname(current->cmd_words[ind_word]);
			if (ret == -1)
				return (1);
			else if (ret == 0)
				*count += 1;
			else
				*count += ret;
		}
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
	int		ind_word;
	int		ind_new;
	int		ind_new_prev;

	ind_word = 0;
	ind_new = 0;
	ind_new_prev = 0;
	while (ind_word < current->nb_cmd_words)
	{
		ft_printf(LOGSV, "         \t\tPathname expansion on word %s\n",
			current->cmd_words[ind_word]);
		if (ft_strchrp_nonquoted(current->cmd_words[ind_word], '*', 2, 0) == -1)
		{
			ft_printf(LOGSV, "         \t\t-> no wildcard detected\n");
			res[ind_new] = ft_strdup(current->cmd_words[ind_word]);
			if (!res[ind_new])
				return (1);
			ind_new++;
		}
		else
		{
			if (ft_parse3_store_pathname(current->cmd_words[ind_word],
				res, &ind_new))
				return (1);
			if (ind_new_prev == ind_new)
			{
				ft_printf(LOGSV, "         \t\t-> no match, taken as it is\n");
				res[ind_new] = ft_strdup(current->cmd_words[ind_word]);
				if (!res[ind_new])
					return (1);
				ind_new++;
			}
		}
		ind_new_prev = ind_new;
		ind_word++;
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