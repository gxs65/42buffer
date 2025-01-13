/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_initialize_data.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 17:28:36 by ilevy             #+#    #+#             */
/*   Updated: 2025/01/10 18:29:17 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../h_files/ft_minishell.h"

int	ft_initialize_data(t_data *data, char **env)
{
	t_token	*tokens;

	tokens = (t_token *)malloc(1 * sizeof(t_token));
	data = (t_data *)malloc(1 * sizeof(t_data));
	if (!data || !tokens)
		return (-1);
	tokens->name = NULL;
	tokens->type = NULL;
	data->env = env;
	data->filefd_out = -42;
	data->filefd_in = -42;
	return (0);
}
