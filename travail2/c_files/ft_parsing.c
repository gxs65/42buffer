/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 18:41:52 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/10 02:38:03 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/ft_minishell.h"

int ft_open_quotes(char *line);

int ft_open_quotes(char *line)
{
	int	i;
	int db;
	int	sg;

	i = 0;
	db = 0;
	sg = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !db)
			sg = !sg;
		else if (line[i] == '\"' && !sg)
			db = !db;
		i++;
	}
	if (db || sg)
	{
		ft_printf(2, "Invalid line: Open quotes");
		return (1);
	}
	return (0);
}
