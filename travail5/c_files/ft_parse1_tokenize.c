/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse1_tokenize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 03:59:35 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/20 23:50:04 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/ft_minishell.h"

char	**ft_create_tokens(char *line);
int		*ft_create_types(char **cmd);
int		ft_token_amount(char *line);
char	*ft_get_token(char *line, unsigned int *i);
int		ft_get_type(char **cmd, unsigned int i);

char	**ft_create_tokens(char *line)
{
	unsigned int		i;
	unsigned int		current_position;
	char				**tokens;
	unsigned int		token_count;

	token_count = ft_token_amount(line);
	tokens = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (!tokens)
		return (NULL);
	i = 0;
	current_position = 0;
	while (i < token_count)
	{
		tokens[i] = ft_get_token(line, &current_position);
		i++;
	}
	tokens[i] = NULL;
	return (tokens);
}

int	*ft_create_types(char **cmd)
{
	unsigned int	i;
	unsigned int	total;
	int	*types;
	int	cmd_flag;

	total = 0;
	cmd_flag = 0;
	while (cmd[total])
		total++;
	types = (int *)malloc(sizeof(int) * (total + 1));
	if (!types)
		return (NULL);
	i = 0;
	while (i < total)
	{
		types[i] = ft_get_type(cmd, i);
		i++;
	}
	types[i] = -42;
	return (types);

}

int		ft_get_type(char **cmd, unsigned int i)
{
	int	type;

	type = 0;
	if (!ft_strncmp(cmd[i], ")", 2))
		type = CLOSE;
	else if (!ft_strncmp(cmd[i], "(", 2))
		type = OPEN;
	else if (!ft_strncmp(cmd[i], "||", 3))
		type = OR;
	else if (!ft_strncmp(cmd[i], "|", 2))
		type = PIPE;
	else if (!ft_strncmp(cmd[i], ">", 2))
		type = TRUNC;
	else if (!ft_strncmp(cmd[i], "<", 2))
		type = INPUT;
	else if (!ft_strncmp(cmd[i], "<<", 3))
		type = HEREDOC;
	else if (!ft_strncmp(cmd[i], ">>", 3))
		type = APPEND;
	else if (!ft_strncmp(cmd[i], "&&", 3))
		type = AND;
	else if (!ft_strncmp(cmd[i], "=", 2))
		type = EQUAL;
	else
		type = CMD;
	return (type);
}

char	*ft_get_token(char *line, unsigned int *i)
{
	int		start;
	int		end;
	char	quote;

	if (line[*i])
	{
		while (ft_isspace(line[*i])) //Skip whitespaces
			(*i)++;
		if (!line[*i]) // Check you didnt finish the command
			return (NULL);
		if (ft_strchr("|&()><=", line[*i])) //Check for metacharacters to be special tokens,
		{
			start = *i;
			end = start;
			if (ft_same_char(line[start], line[start + 1]) &&
			!(line[start] == '(' || line[start] == ')' || line[start] == '='))
				end++;
			*i = end + 1;
		}
		else //Check for regular words and quoted strings.
		{
			start = *i;
			end = start;
			while (line[end] && !ft_isspace(line[end]) && !ft_strchr("|&()><=", line[end])) // While you dont find a meta character
			{
				if (line[end] == '\'' || line[end] == '\"') //If you find quotes skip to the end of the opposing quote
				{
					quote = line[end];
					end++;
					while (line[end] && line[end] != quote)
						end++;
					if (line[end])
						end++;
				}
				else //Otherwise just move forward until you find a metacharacter or a space or end of command.
					end++;
			}
			end--;
			*i = end + 1;
		}
		return (ft_substr(line, start, end - start + 1));
	}
	return (NULL);
}

int	ft_token_amount(char *line)
{
	int			i;
	int			amount;
	char		quote;

	i = 0;
	amount = 0;
	while (line[i])
	{
		while (ft_isspace(line[i])) // Skip whitespace
			i++;
		if (!line[i]) //Check end of command
			break;
		if (ft_strchr("|&()><=", line[i])) //If meta caracter
		{
			amount++;
			if (ft_same_char(line[i], line[i + 1]) && !(line[i] == '(' || line[i] == ')' || line[i] == '='))
				i++;
		}
		else if (line[i] && !ft_isspace(line[i]) && !ft_strchr("|&()><=", line[i]))
		{
			amount++;
			while (line[i] && !ft_isspace(line[i]) && !ft_strchr("|&()><=", line[i]))
			{
				if (line[i] == '\'' || line[i] == '\"')
				{
					quote = line[i];
					i++;
					while (line[i] && line[i] != quote)
						i++;
				}
				else
					i++;
			}
			if (ft_strchr("|&()><=", line[i]))
				i--;
		}
		i++;
	}
	return (amount);
}

// Uses readline to get the next line entered by the user,
// and creates the 2 arrays containing this line's tokens' names and types
int	ft_parse1_tokenize(t_data *data)
{
	char	*line;

	line = readline("\033[1;32m>>>\033[0m ");
	if (!line)
		return (KILL_MALLOC_ERROR);
	add_history(line);
	data->tokens->name = ft_create_tokens(line);
	if (!data->tokens->name)
	{
		free(line);
		return (KILL_MALLOC_ERROR);
	}
	data->tokens->type = ft_create_types(data->tokens->name);
	if (!data->tokens->type)
	{
		free_strs_tab_nullterm(data->tokens->name);
		free(line);
		return (KILL_MALLOC_ERROR);
	}
	free(line);
	return (0);
}