/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 03:59:35 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/10 16:35:21 by ilevy            ###   ########.fr       */
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
	if (!ft_strncmp(cmd[i], ")", 1))
		type = CLOSE;
	else if (!ft_strncmp(cmd[i], "(", 1))
		type = OPEN;
	else if (!ft_strncmp(cmd[i], "||", 2))
		type = OR;
	else if (!ft_strncmp(cmd[i], "|", 1))
		type = PIPE;
	else if (!ft_strncmp(cmd[i], ">", 1))
		type = TRUNC;
	else if (!ft_strncmp(cmd[i], "<", 1))
		type = INPUT;
	else if (!ft_strncmp(cmd[i], "<<", 2))
		type = HEREDOC;
	else if (!ft_strncmp(cmd[i], ">>", 2))
		type = APPEND;
	else if (!ft_strncmp(cmd[i], "&&", 2))
		type = AND;
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
		while (ft_isspace(line[*i]))
			(*i)++;
		if (!line[*i])
			return (NULL);
		if (line[*i] == '\'' || line[*i] == '\"')
		{
			start = *i;
			end = start + 1;
			quote = line[start];
			if (line[start + 1] == quote)
			{
				ft_printf(2, "\"\" and \'\' are unknown commands.\n");
				return (NULL);
			}
			while (line[end] && line[end] != quote)
				end++;
			*i = end + 1;
		}
		else if (ft_strchr("|&()><", line[*i]))
		{
			start = *i;
			end = start;
			if (ft_same_char(line[start], line[start + 1]) && !(line[start] == '(' || line[start] == ')'))
				end++;
			*i = end + 1;
		}
		else if (line[*i] && !ft_isspace(line[*i]) && !ft_strchr("|&()><", line[*i]))
		{
			start = *i;
			end = start;
			while (line[end] && !ft_isspace(line[end]) && !ft_strchr("|&()><", line[end]))
				end++;
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
		while (ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i];
			if (line[i + 1] == quote)
			{
				ft_printf(2, "\"\" and \'\' are unknown commands.\n");
				break ;
			}
			amount++;
			i++;
			while (line[i] && line[i] != quote) 
				i++;
		}
		else if (ft_strchr("|&()><", line[i]))
		{
			amount++;
			if (ft_same_char(line[i], line[i + 1]) && !(line[i] == '(' || line[i] == ')'))
				i++;
		}
		else if (line[i] && !ft_isspace(line[i]) && !ft_strchr("|&()><", line[i]))
		{
			amount++;
			while (line[i] && !ft_isspace(line[i]) && !ft_strchr("|&()><", line[i]))
				i++;
			if (ft_strchr("|&()><", line[i]))
				i--;
		}
		i++;
	}
	return (amount);
}
