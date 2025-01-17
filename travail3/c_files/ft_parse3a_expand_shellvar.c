#include "../h_files/ft_minishell.h"

// Searches the <data.env> string array
// (where each element is of the form "shell_varname=shellvar_value")
// for a line with varname identical to <name>,
// 		and returns a pointer to the shellvar_value string inside <data.env>
char	*ft_parse3_find_sv(t_data *data, char *name, int len_sv)
{
	t_var	*var;

	ft_printf(LOGSV, "         \t<find_shellvar> on %s with len %d\n",
		name, len_sv);
	var = data->env;
	while (var)
	{
		if (ft_strncmp(var->name, name, len_sv) == 0)
			return (var->value);
		var = var->next;
	}
	ft_printf(LOGSV, "         \t<find_shellvar> found nothing\n");
	return (NULL);
}

// Returns the length of the substring, from string <token>,
// that is a shell variable name
// 		(the shell variable name begins at index 0)
// Rules on shell variables names : only characters from A-Za-z0-9 and '_',
// 		+ may not begin with a character from 0-9
// -> to isolate the shell variable name substring,
// 		go through the token until finding
// 			\ the character '\0'
// 			\ a character not from A-Za-z0-9 and '_'
// 			\ a character from 0-9, and that character is at index 0 
int		ft_parse3_calc_len_sv(char *token)
{
	int	len;

	len = 0;
	while (token[len] && (ft_isalnum(token[len]) || token[len] == '_')
		&& !(len == 0 && ft_isdigit(token[len])))
		len++;
	return (len);
}

// Replaces, in the string of token located at <pos_tok> in <data.tokens.name>,
// the name of the shell variable beginning at index <ind_sv>
// by the value of this shell variable given by <value_sv>
// 		/!\ allocates a new string, frees the old string in <data.tokens.name>,
// 			and places a pointer to the new string in place of the old string
// 		/!\ <len_sv> is the length of the shell varable name,
// 			not the length of the shell variable value
// 		/!\ computation of <len_newtok> : length of original token,
// 			plus length of shell variable value, minus length of variable name,
// 			minus 1 to account for the '$' character (not counted in <len_sv>)
int	ft_parse3_substitute_sv(char **tok, char *value_sv, int ind_sv, int len_sv)
{
	char	*swapper;
	int		len_newtok;

	len_newtok = ft_strlen(*tok) + ft_strlen(value_sv) - len_sv - 1;
	swapper = malloc((len_newtok + 1) * sizeof(char));
	if (!swapper)
		return (1);
	swapper[0] = '\0';
	ft_strncat(swapper, *tok, ind_sv);
	ft_strcat(swapper, value_sv);
	ft_strcat(swapper, *tok + ind_sv + 1 + len_sv);
	free(*tok);
	*tok = swapper;
	return (0);
}

// Replaces, in the string of token located at <pos_tok> in <data.tokens.name>,
// the name of the shell variable beginning at index <ind_sv>
// by nothing (because shell variable with that name has not been found)
int	ft_parse3_remove_sv(char **tok, int ind_sv, int len_sv)
{
	char	*swapper;
	int		len_newtok;

	len_newtok = ft_strlen(*tok) - len_sv - 1;
	swapper = malloc((len_newtok + 1) * sizeof(char));
	if (!swapper)
		return (1);
	swapper[0] = '\0';
	ft_strncat(swapper, *tok, ind_sv);
	ft_strcat(swapper, *tok + ind_sv + 1 + len_sv);
	free(*tok);
	*tok = swapper;
	return (0);
}

// Handles the three possible cases when encountering a '$' in a token :
// 		- <len_sv> > 0 (there is a shell variable name of at least 1 character)
// 		  and <value_sv> is defined (shell variable name found in env)
// 			-> replace the shell variable name by its value
// 		- <len_sv> > 0 and <value_sv> is NULL (name not found in env)
// 			-> replace shell variable name by nothing
// 		- <len_sv> == 0 (a single '$')
// 			-> do nothing, let the dollar character standing
// + updates the <ind_current> according to the case
int	ft_parse3_handle_sv_at(t_data *data, int pos_tok,
	int ind_sv, int *ind_current)
{
	int		len_sv;
	char	*value_sv;

	len_sv = ft_parse3_calc_len_sv(data->tokens->name[pos_tok]
		+ ind_sv + 1);
	if (len_sv > 0)
	{
		value_sv = ft_parse3_find_sv(data,
			data->tokens->name[pos_tok] + ind_sv + 1, len_sv);
		if (value_sv && ft_parse3_substitute_sv(
			&(data->tokens->name[pos_tok]), value_sv, ind_sv, len_sv))
			return (1);
		else if (value_sv)
			*ind_current = ind_sv + ft_strlen(value_sv);
		else if (ft_parse3_remove_sv(
			&(data->tokens->name[pos_tok]), ind_sv, len_sv))
			return (1);
		else
			*ind_current = ind_sv;
	}
	else
		*ind_current = ind_sv + 1;
	return (0);
}

// Expands all shell variable names in a token
// 		- repeats expanding 1 '$' on the same token
// 			by looping until no new '$' character is found <=> <ind_sv> == -1
// 		- updates at each cycle a <ind_current>,
//  		the index at which <ft_strchrp> must begin its search for '$',
// 				so as to avoid expanding '$' in results of previous expansions
// 		- uses <ft_ftrchrp_nonquoted> to avoid quoted '$' characters,
// 			with parameter level=1 to allow double quoted '$'
// 			and forbid single quoted '$'
int	ft_parse3_expand_sv_on_tok(t_data *data, int pos_tok)
{
	int		ind_current;
	int		ind_sv;

	ind_current = 0;
	ind_sv = ft_strchrp_nonquoted(data->tokens->name[pos_tok],
		'$', 1, ind_current);
	while (ind_sv != -1)
	{
		ft_printf(LOGSV, "         \t\t<loop> ind_curr %d, ind_sv %d, tok %s\n",
			ind_current, ind_sv, data->tokens->name[pos_tok]);
		if (ft_parse3_handle_sv_at(data, pos_tok, ind_sv, &ind_current))
			return (1);
		ind_sv = ft_strchrp_nonquoted(data->tokens->name[pos_tok],
			'$', 1, ind_current);
	}
	ft_printf(LOGS, "         \tfinal state of token index %d : %s\n",
		pos_tok, data->tokens->name[pos_tok]);
	return (0);
}
