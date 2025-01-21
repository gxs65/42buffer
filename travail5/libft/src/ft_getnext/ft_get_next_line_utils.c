/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:09:52 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/30 21:38:45 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

int		get_strlen(char *str);
char	*get_strchr(char *str, int i_c);
char	*get_strjoin(char *src, char *add);
void	get_bzero(void *s, size_t n);
void	*get_calloc(size_t count, size_t size);

int	get_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*get_strchr(char *str, int i_c)
{
	int		i;
	char	c;

	i = 0;
	c = (char) i_c;
	while (str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	if (str[i] == c)
		return (&str[i]);
	return (NULL);
}

char	*get_strjoin(char *src, char *add)
{
	int		i;
	int		j;
	int		mover;
	char	*join;

	i = get_strlen(src);
	j = get_strlen(add);
	mover = 0;
	join = (char *)malloc((1 + i + j) * sizeof(char));
	if (!join || !src || !add)
		return (NULL);
	while (mover < i)
	{
		join[mover] = src[mover];
		mover++;
	}
	mover = 0;
	while (mover < j)
	{
		join[i + mover] = add[mover];
		mover++;
	}
	join[mover + i] = '\0';
	return (join);
}

void	get_bzero(void *s, size_t n)
{
	char	*str;
	size_t	i;

	i = 0;
	str = (char *)s;
	while (i < n)
		str[i++] = '\0';
}

void	*get_calloc(size_t count, size_t size)
{
	char	*cameleon;

	cameleon = (char *)malloc(count * size);
	if (!cameleon)
		return (NULL);
	get_bzero(cameleon, count * size);
	return (cameleon);
}
