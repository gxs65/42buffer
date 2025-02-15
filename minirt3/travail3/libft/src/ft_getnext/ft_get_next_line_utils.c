/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:33:57 by abedin            #+#    #+#             */
/*   Updated: 2025/01/23 02:28:46 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/ft_get_next_line.h"

// Creates a new node for a chained list of strings
// (the node has a <fd> identifier, and a <data> string of size BUGGER_SIZE)
t_cstr	*ft_create_cstr(char *data, int fd)
{
	t_cstr	*cstr;

	cstr = malloc(1 * sizeof(t_cstr));
	if (!cstr)
		return (NULL);
	cstr->data[BUFFER_SIZE] = '\0';
	cstr->fd = fd;
	ft_strncpy(cstr->data, data, BUFFER_SIZE);
	cstr->next = NULL;
	return (cstr);
}

// Deletes the elements of a linked list whose <cstr->fd> equals <fd>
// (therefore : frees all elements of a list storing chunks of read line,
//		but frees only one element of a list storing the <prests>
//		of different files)
t_cstr	*free_cstrs(t_cstr *cstr, int fd)
{
	t_cstr	*next;

	if (cstr)
	{
		next = cstr->next;
		if (fd == cstr->fd)
		{
			free(cstr);
			return (free_cstrs(next, fd));
		}
		else
		{
			if (next)
				cstr->next = free_cstrs(next, fd);
			return (cstr);
		}
	}
	else
		return ((t_cstr *) 0);
}

// Finds an element with the given <fd> in the list <prest>,
//		or creates one at the front of the list if there is none
t_cstr	*find_cstr_by_fd(t_cstr **prest, int fd)
{
	t_cstr	*found;

	found = *prest;
	while (found && found->fd != fd)
		found = found->next;
	if (!(found && found->fd == fd))
	{
		found = *prest;
		*prest = ft_create_cstr("", fd);
		(*prest)->next = found;
		found = *prest;
	}
	return (found);
}

// Does not return a pointer to the first occurrence of <to_fin> like strchr,
//		but the index (int) of that character (or -1 if not found)
// + processes the terminating null byte,
//		so that it may be used as a strlen
int	ft_strchrp_gnl(char *str, char to_find)
{
	int	pos;

	if (str)
	{
		pos = 0;
		while (str[pos])
		{
			if (str[pos] == to_find)
				return (pos);
			pos++;
		}
		if (to_find == '\0')
			return (pos);
	}
	return (-1);
}

// Standard function to copy the <n> first characters of <src> to <dest>
// and add a terminating null byte
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
