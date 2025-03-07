#include "../h_files/ft_minishell.h"

// Computes the length of the token beginning at <pos> in <str>
// 		- considering space (' ', ascii 9 to 13) characters as separators
// 		- skipping space characters enclosed in single or double quotes
int	ft_go_through_token_space(const char *str, int pos)
{
	int		token_len;
	char	quoted;

	quoted = ' ';
	token_len = 0;
	while (str[pos + token_len] && !ft_isspace(str[pos + token_len]))
	{
		if ((str[pos + token_len] == '\'') || (str[pos+ token_len] == '\"'))
		{
			quoted = str[pos + token_len];
			token_len++;
			while (str[pos + token_len] && str[pos + token_len] != quoted)
				token_len++;
			if (str[pos + token_len] && str[pos + token_len] == quoted)
				token_len++;
		}
		else
			token_len++;
	}
	return (token_len);
}

// Counts the tokens of string <str> separated by unquoted space characters
int	ft_count_tokens_space(const char *str)
{
	int		token_len;
	int		token_count;
	int		pos;

	token_count = 0;
	pos = 0;
	while (str[pos])
	{
		if (!ft_isspace(str[pos]))
		{
			token_len = ft_go_through_token_space(str, pos);
			pos = pos + token_len;
			token_count++;
		}
		else
			pos++;
	}
	return (token_count);
}

// Stores the tokens of string <str> separated by unquoted space characters
// in the string array (that must be of the adequate size) <res>
int	ft_store_tokens_space(const char *str, char **res)
{
	int		token_len;
	int		token_count;
	int		pos;

	token_count = 0;
	pos = 0;
	while (str[pos])
	{
		if (!ft_isspace(str[pos]))
		{
			token_len = ft_go_through_token_space(str, pos);
			res[token_count] = ft_strndup(str + pos, token_len);
			if (!res[token_count])
				return (free_strs_tab_upto(res, token_count));
			pos = pos + token_len;
			token_count++;
		}
		else
			pos++;
	}
	return (0);
}

// Splits a string <s> by unquoted space characters,
// and returns the resulting array of tokens allocated using <malloc>
char	**ft_split_space(const char *s)
{
	char	**res;
	int		token_count;

	token_count = ft_count_tokens_space(s);
	res = malloc((token_count + 1) * sizeof(char *));
	if (!res)
		return ((char **) 0);
	res[token_count] = (char *) 0;
	if (token_count > 0)
	{
		if (ft_store_tokens_space(s, res))
			return ((char **) 0);
	}
	return (res);
}
