/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:34:32 by abedin            #+#    #+#             */
/*   Updated: 2025/01/23 02:29:01 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/ft_get_next_line.h"

// Duplicates the line contained in the <prest> string,
// then returns that line and reweites <prest> to remove
// the extracted line from it
int	manage_line_in_rest(char **ret, char *prest, int pos_ln)
{
	*ret = malloc((pos_ln + 2) * sizeof(char));
	if (!(*ret))
		return (1);
	(*ret)[pos_ln + 1] = '\0';
	ft_strncpy(*ret, prest, pos_ln + 1);
	ft_strncpy(prest, prest + pos_ln + 1, BUFFER_SIZE);
	return (0);
}

// Constructs the line :
// - receives the beginning of the line in <prest>,
//		and the rest of the line in parts in a chained list <cstr>
// - allocates the needed memory to a <ret> string,
// 		and stitches <prest> and <cstr> as a single line inside <ret>
// - (also with [cstr_to_str]) stores the remainder (beyond the "\n")
//		in <prest> of max <BUFFER_SIZE> characters
// - finally frees the chaines str used to carry the line chunks
// (when no characters left for the line / error of read / error of malloc,
//  returns NULL)
int	manage_line_in_file(char **ret, char *prest, int fd, char *buffer)
{
	int		pos_end;
	t_cstr	*cstr;
	int		len_prest;

	len_prest = ft_strchrp_gnl(prest, '\0');
	pos_end = 0;
	cstr = read_to_ln(fd, &pos_end, buffer);
	if (pos_end < 0 || (pos_end == 0 && len_prest == 0))
		return ((free_cstrs(cstr, -1) == NULL));
	*ret = malloc((len_prest + pos_end + 1) * sizeof(char));
	if (!(*ret))
		return ((free_cstrs(cstr, -1) == NULL));
	(*ret)[len_prest + pos_end] = '\0';
	ft_strncpy(*ret, prest, BUFFER_SIZE);
	if (pos_end > 0)
		cstr_to_str(cstr, prest, *ret + len_prest, pos_end);
	else
		prest[0] = '\0';
	return ((free_cstrs(cstr, -1) == NULL));
}

// Cobbles together the <data> strings of all nodes in <cstr> linked list
// by recursively going through the nodes and appending their content to <ret>
// 		(when coming to last node, the part after '\n' is copied to <prest>)
void	cstr_to_str(t_cstr *cstr, char *prest, char *ret, int pos_end)
{
	int		len_elem;

	len_elem = ft_strchrp_gnl(cstr->data, '\0');
	if (len_elem >= pos_end)
	{
		ft_strncpy(ret, cstr->data, pos_end);
		ft_strncpy(prest, cstr->data + pos_end, BUFFER_SIZE);
	}
	else
	{
		ft_strncpy(ret, cstr->data, BUFFER_SIZE);
		if (cstr->next)
			cstr_to_str(cstr->next, prest, ret + BUFFER_SIZE,
				pos_end - BUFFER_SIZE);
	}
}

// Reads a file by chunks of <BUFFER_SIZE> characters,
// 		stopping at the first chunk containing a "\n"
// 		or containing less than <BUFFER_SIZE> characters
// - if there are characters to read, returns the chunks
// 		as a chained list of <t_cstr> elements,
//		and sets <pos_end> as the number of characters stored
//		including the final "\n" if there is one
// - if there are no characters to read or an error occurs,
//		return NULL and sets <pos_end> to 0
//		(both cases are treated the same way, cf subject line 2)
t_cstr	*read_to_ln(int fd, int *pos_end, char *buffer)
{
	int		nb_read;
	t_cstr	*curr_cstr;
	int		pos_ln;

	nb_read = read(fd, buffer, BUFFER_SIZE);
	if (nb_read > 0)
	{
		buffer[nb_read] = '\0';
		curr_cstr = ft_create_cstr(buffer, -1);
		if (!curr_cstr)
		{
			*pos_end = -1;
			return (NULL);
		}
		pos_ln = ft_strchrp_gnl(buffer, '\n');
		if (pos_ln != -1)
			*pos_end += pos_ln + 1;
		else
			*pos_end += nb_read;
		if (pos_ln == -1 && nb_read == BUFFER_SIZE)
			curr_cstr->next = read_to_ln(fd, pos_end, buffer);
		return (curr_cstr);
	}
	return (NULL);
}

// (general summary of the functions)
// Reads one line of a file given by file descriptor <fd>
// - first selects the <prest> associated to the given <fd>,
//		stored (if the function has already called for that <fd>)
//		in a chained list of strings
// - if the next line is contained in the remainder <prest>
// 		from the last call, only extracts the line in <prest>
// - else, read the file by chunks of <BUFFER_SIZE> characters,
// 		stopping at the first chunk containing a "\n"
// 		or containing less than <BUFFER_SIZE> characters ;
// - then allocates the needed memory to a <ret> string to be returned,
//		and stores the remainder (beyond the "\n")
//		in a static string <prest> of max <BUFFER_SIZE> characters,
//		which will be used as the next call of the function
// -> <prest> is a static variable initialized only at the first call
// (not able to read binary files, undefined behavior in that case)
// (buffer limited to 5 000 000, to avoid a stack overflow error)
// Correction to avoid memory leaks in case of file not read up to the end :
// 		- <mode> set to 0 indicates that the function should act normally
// 		- <mode> set to 1 indicates that the function should only free
// 		  the memory used to remember the <prest> associated with file <fd>
// 		  	(<=> free the node with the given fd in the linked list <prest>)
char	*get_next_line(int fd, int mode)
{
	int				pos_ln;
	static t_cstr	*prest;
	t_cstr			*curr_cstr;
	char			*ret;
	char			*buffer;

	if (mode == 1)
		return (prest = free_cstrs(prest, fd), NULL);
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	curr_cstr = find_cstr_by_fd(&prest, fd);
	ret = NULL;
	pos_ln = ft_strchrp_gnl(curr_cstr->data, '\n');
	if (pos_ln != -1)
		(void)manage_line_in_rest(&ret, curr_cstr->data, pos_ln);
	else
		(void)manage_line_in_file(&ret, curr_cstr->data, fd, buffer);
	if (!ret)
		prest = free_cstrs(prest, fd);
	free(buffer);
	return (ret);
}

// Log lines

/*
printf("\n[get_next_line] -> node selected :\n");
display_elem(curr_cstr);
printf("\n[get_next_line] : \
	selecting prest with fd %d among chained list :\n", fd);
*/

/*
void	display_elem(t_cstr *elem)
{
	if (elem)
		printf("<addr %p : data = %s, fd = %d, next = %p>\n",
			elem, elem->data, elem->fd, elem->next);
	else
		printf("\n");
}

void	display_list(t_cstr *elem)
{
	if (elem)
	{
		display_elem(elem);
		display_list(elem->next);
	}
	else 
		printf("-End of list\n");
}
*/

//if (ret && strlen(ret) < 20)
		//printf("RETURNING (bufsize = %i) : '%s'\n", BUFFER_SIZE, ret);
	//else if (ret)
		//printf("RETURNING line too long\n");
	//else
		//printf("RETURNING NULL\n");
	//printf("State of PRESTS :\n");
	//display_list(prest);
