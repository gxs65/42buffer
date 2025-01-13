/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_line_to_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <ilevy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:22:26 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/10 04:00:07 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/ft_minishell.h"

int 	ft_line_to_data(char *line);

int ft_line_to_data(char *line)
{
	int		*types;
	char	**cmd;

	if (ft_open_quotes(line)) // Check that the line is not open.
        return (-1);
	cmd = ft_create_tokens(line);
	if (!cmd)
	{
		free(line);
		return (-1);
	}
	types = ft_create_types(cmd);
	if (!types)
	{
		// ft_free_tab_void(cmd);
		free(line);
		return (-1);
	}
	return (1);
}
