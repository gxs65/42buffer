#include "../h_files/ft_minishell.h"

// Searches the <data.env> string array
// (where each element is of the form "shell_varname=shellvar_value")
// for a line with varname identical to <name>,
// 		and returns a pointer to the shellvar_value string inside <data.env>
char	*ft_parse3_find_shellvar(t_data *data, char *name, int len_shellvar)
{
	int		ind_in_env;
	char	*line;

	ft_printf(LOGSV, "[PARSE3] \t<find_shellvar> on %s with len %d\n",
		name, len_shellvar);
	ind_in_env = 0;
	while (data->env[ind_in_env])
	{
		line = data->env[ind_in_env];
		if (ft_strlen(line) > len_shellvar && line[len_shellvar] == '='
			&& strncmp(name, data->env[ind_in_env], len_shellvar) == 0)
			return (data->env[ind_in_env] + len_shellvar + 1);
		ind_in_env++;
	}
	return (NULL);
}

// Returns the length of the substring, from string <token>,
// that is a shell variable name
// 		(the shell variable name begins at index 0)
// Rules on shell variables names : only characters from A-Za-z0-9 and '_',
// 		+ may not begin with a character from 0-9
// -> to isolate the shell variable name substring,
// 		go through the token until finding a character
// 			\ the character '\0'
// 			\ a character not from A-Za-z0-9 and '_'
// 			\ a character from 0-9, and that character is at index 0 
int		ft_parse3_calc_len_shellvar(char *token)
{
	int	len;

	len = 0;
	while (token[len] && (ft_isalnum(token[len]) || token[len] == '_')
		&& !(len == 0 && ft_isnum(token[len])))
		len++;
	return (len);
}

int	ft_parse3_substitute_shellvar(t_data *data, int *tok,
	char *shellvar_value, int *ind_current)
{
	char	*swapper;

	swapper = ft_strndup(tok, ind_shellvar);
	swapper = ft_strjoin(swapper, shellvar_value); // TODO : fonction strjoin qui free s1
	swapper = ft_strjoin(swapper, tok + ind_shellvar + 1 + len_shellvar);
	ind_current = ind_shellvar + ft_strlen(shellvar_value);
	free(tok);
	tok = swapper;
}

int	ft_parse3_remove_shellvar(t_data *data, int *tok, int *ind_current)
{
	char	*swapper;

	swapper = ft_strndup(tok, ind_shellvar);
	swapper = ft_strjoin(swapper, tok + ind_shellvar + len_shellvar + 1);
	ind_current = ind_shellvar;
	free(tok);
	tok = swapper;
}

int	ft_parse3_handle_shellvar_at(t_data *data, int pos_tok,
	int ind_shellvar, int *ind_current)
{
		int		len_shellvar;
len_shellvar = ft_parse3_calc_len_shellvar(tok + ind_shellvar + 1);
		if (len_shellvar > 0)
		{
			shellvar_value = ft_parse3_find_shellvar(data,
				tok + ind_shellvar + 1, len_shellvar);
			if (shellvar_value && ft_parse3_substitute_shellvar(data,
				tok, shellvar_value, &ind_current))
				return (1);
			else if (ft_parse3_remove_shellvar(data, tok, &ind_current))
				return (1);
		}
		else
			ind_current = ind_shellvar + 1;

int	ft_parse3_expand_shellvar_on_tok(t_data *data, int pos_tok)
{
	int		ind_current;
	int		ind_shellvar;
	char	*tok;
	char	*shellvar_value;

	tok = data->tokens->name[pos_tok];
	ft_printf(LOGSV, "[PARSE3] \tShellvar expansion on token index %d : %s\n",
		pos_tok, tok);
	ind_current = 0;
	ind_shellvar = ind_current + ft_strchrp(tok + ind_current, '$');
	while (ind_shellvar != -1)
	{
		ft_printf(LOGSV, "         \tind_curr %d, ind_shellvar %d, tok %s\n",
			ind_current, ind_shellvar, tok);
		ft_parse3_handle_shellvar_at(data, pos_tok, ind_shellvar, &ind_current);
		ind_shellvar = ft_strchrp(tok + ind_current, '$');
		if (ind_shellvar != -1)
			ind_shellvar += ind_current;
	}
	ft_printf(LOGS, "         \tfinal state of token index %d : %s\n",
		pos_tok, tok);
	return (0);
}

/*
// <tok> is the index of the token to analyze in <data.tokens.name>
int	ft_parse3_expand_shellvar(t_data *data, int pos_tok)
{
	int		ind_current;
	int		ind_shellvar;
	int		len_shellvar;
	char	*swapper;
	char	*tok;
	char	*shellvar_value;

	tok = ft_strdup(data->tokens->name[pos_tok]);
	ft_printf(LOGSV, "[PARSE3] \tShellvar expansion on token index %d : %s\n",
		pos_tok, tok);
	ind_current = 0;
	ind_shellvar = ind_current + ft_strchrp(tok + ind_current, '$');
	while (ind_shellvar != -1)
	{
		ft_printf(LOGSV, "         \tind_curr %d, ind_shellvar %d, tok %s\n",
			ind_current, ind_shellvar, tok);
		len_shellvar = ft_parse3_calc_len_shellvar(tok + ind_shellvar + 1);
		if (len_shellvar > 0)
		{
			shellvar_value = ft_parse3_find_shellvar(data, tok + ind_shellvar + 1, len_shellvar);
			if (shellvar_value)
			{
				swapper = ft_strndup(tok, ind_shellvar);
				swapper = ft_strjoin(swapper, shellvar_value); // TODO : fonction strjoin qui free s1
				swapper = ft_strjoin(swapper, tok + ind_shellvar + 1 + len_shellvar);
				ind_current = ind_shellvar + ft_strlen(shellvar_value);
				free(tok);
				tok = swapper;
			}
			else
			{
				swapper = ft_strndup(tok, ind_shellvar);
				swapper = ft_strjoin(swapper, tok + ind_shellvar + len_shellvar + 1);
				ind_current = ind_shellvar;
				free(tok);
				tok = swapper;
			}
		}
		else
			ind_current = ind_shellvar + 1;
		if (ft_strchrp(tok + ind_current, '$') == -1)
			ind_shellvar = -1;
		else
			ind_shellvar = ind_current + ft_strchrp(tok + ind_current, '$');
	}
	ft_printf(LOGS, "         \tfinal state of token index %d : %s\n",
		pos_tok, tok);
	return (0);
}
*/