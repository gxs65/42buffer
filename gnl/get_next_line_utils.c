/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_cstrs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:37:57 by abedin            #+#    #+#             */
/*   Updated: 2024/07/17 16:37:59 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_cstr	*ft_create_cstr(char *data)
{
	t_cstr	*cstr;

	cstr = malloc(1 * sizeof(t_cstr));
	if (!cstr)
		return (NULL);
	cstr->data[M_BUFFER_SIZE] = '\0';
	ft_strncpy(cstr->data, data, M_BUFFER_SIZE);
	cstr->next = NULL;
	return (cstr);
}

void	cstr_to_str(t_cstr *cstr, char *str, char *rest)
{
	int		pos_ln;

	if (cstr)
	{
		pos_ln = ft_char_in_str(cstr->data, '\n');
		if (pos_ln != -1)
		{
			ft_strncpy(str, cstr->data, pos_ln);
			ft_strncpy(rest, cstr->data + pos_ln + 1,
				M_BUFFER_SIZE - pos_ln - 1);
		}
		else
		{
			ft_strncpy(str, cstr->data, M_BUFFER_SIZE);
			if (cstr->next)
				cstr_to_str(cstr->next, str + M_BUFFER_SIZE, rest);
		}
	}
}

void	free_cstrs(t_cstr *cstr)
{
	if (cstr)
	{
		free(cstr->data);
		if (cstr->next)
			free_cstrs(cstr->next);
	}
}

int	ft_char_in_str(char *str, char to_find)
{
	int	pos;

	if (str)
	{
		pos = 0;
		while (str[pos])
		{
			//printf("[char_in_str] viewing char %c\n", str[pos]);
			if (str[pos] == to_find)
				return (pos);
			pos++;
		}
	}
	return (-1);
}

void	ft_strncpy(char *dest, char *src, int n)
{
	int	pos;

	pos = 0;
	if (src)
	{
		while (pos < n && src[pos])
		{
			dest[pos] = src[pos];
			pos++;
		}
	}
	dest[pos] = '\0';
}

int	ft_strlen(char *str)
{
	int	i;

	if (str)
	{
		i = 0;
		while (str[i])
			i++;
		return (i);
	}
	else
		return (0);
}
