/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_read.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:55:14 by abedin            #+#    #+#             */
/*   Updated: 2024/07/16 14:55:15 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	manage_line_in_rest(char **ret, char *prest, int pos_ln)
{
	*ret = malloc((pos_ln + 1) * sizeof(char));
	if (!(*ret))
		return (1);
	(*ret)[pos_ln] = '\0';
	ft_strncpy(*ret, prest, pos_ln);
	printf("\t[manage_line_in_rest] copy in ret successful\n");
	ft_strncpy(prest, prest + pos_ln + 1, M_BUFFER_SIZE - pos_ln - 1);
	printf("\t[manage_line_in_rest] new rest after extraction of line :\n---\n%s\n---\n", prest);
	return (0);
}

int	manage_line_in_file(char **ret, char *prest, int fd)
{
	int		pos_ln;
	t_cstr	*cstr;

	pos_ln = 0;
	cstr = read_to_ln(fd, &pos_ln);
	if (pos_ln == -1)
	{
		free_cstrs(cstr);
		*ret = NULL;
		return (1);
	}
	*ret = malloc((pos_ln + ft_strlen(prest) + 1) * sizeof(char));
	if (!(*ret))
	{
		free_cstrs(cstr);
		return (1);
	}
	(*ret)[pos_ln + ft_strlen(prest)] = '\0';
	ft_strncpy(*ret, prest, M_BUFFER_SIZE);
	printf("\t[manage_line_in_file] copy of REST in ret successful\n");
	cstr_to_str(cstr, *ret + ft_strlen(prest), prest);
	printf("\t[manage_line_in_file] copy of CSTR in ret successful\n");
	printf("\t[manage_line_in_file] new rest after extraction of line :\n---\n%s\n---\n", prest);
	free_cstrs(cstr);
	return (0);
}

// Lit le fichier jusqu'au prochain `\n`, en ajoutant <prest> au debut,
// alloue la taille suffisante dans <*ret> pour y mettre le resultat
// et met les caracteres trouves au dela du \n dans <prest>
// Dans le cas d'un read_to_ln avec les <cstr>,
// total_to_ln avec une valeur de -1 indique qu'une alloc a echoue
// -> il faut free ce que la fonction met a <ret>
char	*get_next_line(int fd)
{
	int			pos_ln;
	static char	*prest;
	char		*ret;

	if (!prest)
	{
		prest = malloc(sizeof(char) * (M_BUFFER_SIZE + 1));
		if (!prest)
			return (NULL);
	}
	pos_ln = ft_char_in_str(prest, '\n');
	printf("\t[get_next_line] ln in rest ? %d\n", pos_ln);
	if (pos_ln != -1)
	{
		manage_line_in_rest(&ret, prest, pos_ln);
	}
	else
	{
		manage_line_in_file(&ret, prest, fd);
	}
	return (ret);
}

t_cstr	*read_to_ln(int fd, int *total_to_ln)
{
	int		nb_read;
	t_cstr	*curr_cstr;
	char	buffer[M_BUFFER_SIZE + 1];
	int		pos_ln;

	nb_read = read(fd, buffer, M_BUFFER_SIZE);
	buffer[nb_read] = '\0';
	if (nb_read > 0)
	{
		printf("[read_to_ln] read %d chars in buffer :\n---\n%s\n---\n", nb_read, buffer);
		curr_cstr = ft_create_cstr(buffer);
		if (!curr_cstr)
		{
			*total_to_ln = -1;
			return (NULL);
		}
		printf("[read_to_ln] cstr created correctly\n");
		pos_ln = ft_char_in_str(buffer, '\n');
		printf("[read_to_ln] pos of first ln in buffer : %d\n", pos_ln);
		*total_to_ln += ((pos_ln != -1) * pos_ln) + \
			((pos_ln == -1) * M_BUFFER_SIZE);
		if (pos_ln == -1)
			curr_cstr->next = read_to_ln(fd, total_to_ln);
		return (curr_cstr);
	}
	*total_to_ln = -1;
	return (NULL);
}
