#include "../h_files/ft_minishell.h"

// Removes (non-quoted) quotes from a string
// The string is "copied on itself" (somewhat like in <memcpy>) :
// at each index <ind_in_word>, if the char here is not a (non-quoted) quote,
// is is copied in the string at index <ind_in_copy>, and <ind_in_copy> incrs
// 		(so <ind_in_copy> can never be higher than <ind_in_word>)
void	ft_parse3_remove_quotes(char *word)
{
	int		ind_in_word;
	int		ind_in_copy;
	char	quoted;
	char	c;

	ind_in_word = 0;
	ind_in_copy = 0;
	quoted = ' ';
	while (word[ind_in_word])
	{
		c = word[ind_in_word];
		if (quoted == ' ' && (c == '\'' || c == '\"'))
			quoted = c;
		else if (quoted != ' ' && c == quoted)
			quoted = ' ';
		else
		{
			word[ind_in_copy] = c;
			ind_in_copy++;
		}
		ind_in_word++;
	}
	word[ind_in_copy] = '\0';
}

// Removes quote from a node's command words ;
// cannot fail since no malloc is used (word copied inside itself)
void	ft_parse3_remove_quotes_cmd(t_data *data, t_node *current)
{
	int	ind;

	ft_printf(LOGSV, "         \td) quote removal command words\n");
	ind = 0;
	while (ind < current->nb_cmd_words)
	{
		ft_printf(LOGSV, "         \t\tword %d : %s\n",
			ind, current->cmd_words[ind]);
		ft_parse3_remove_quotes(current->cmd_words[ind]);
		ft_printf(LOGSV, "         \t\t-> %s\n",
			current->cmd_words[ind]);
		ind++;
	}
}

// Removes quote from a node's redir words ;
// cannot fail since no malloc is used (word copied inside itself)
void	ft_parse3_remove_quotes_redir(t_data *data, t_node *current)
{
	int	ind;

	ft_printf(LOGSV, "         \td) quote removal redir words\n");
	ind = 0;
	while (ind < current->nb_redir_words)
	{
		ft_printf(LOGSV, "         \t\tword %d : %s\n",
			ind, current->redir_words[ind]);
		ft_parse3_remove_quotes(current->redir_words[ind]);
		ft_printf(LOGSV, "         \t\t-> %s\n",
			current->redir_words[ind]);
		ind++;
	}
}