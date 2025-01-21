/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilevy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 11:06:30 by ilevy             #+#    #+#             */
/*   Updated: 2024/11/30 21:38:22 by ilevy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../h_files/libft.h"

char	*get_free(char *buffer, char *add);
char	*get_next_line(int fd);
char	*get_read_file(int fd, char **buffer);
char	*gt_line(char *buffer);
char	*get_next(char *buffer);

char	*get_free(char *buffer, char *add)
{
	char	*joined;

	joined = get_strjoin(buffer, add);
	free(buffer);
	return (joined);
}

char	*get_next_line(int fd)
{
	static char	*buffer[256];
	char		*line;

	if (BUFFER_SIZE <= 0 || fd < 0 || fd > 256)
		return (NULL);
	buffer[fd] = get_read_file(fd, &buffer[fd]);
	if (!buffer[fd])
		return (NULL);
	line = gt_line(buffer[fd]);
	buffer[fd] = get_next(buffer[fd]);
	return (line);
}

char	*get_read_file(int fd, char **buffer)
{
	char	*line;
	int		bytes_read;

	if (!*buffer)
		*buffer = get_calloc(1, 1);
	line = get_calloc(BUFFER_SIZE + 1, sizeof(char));
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, line, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(line);
			free(*buffer);
			return (NULL);
		}
		line[bytes_read] = '\0';
		*buffer = get_free(*buffer, line);
		if (get_strchr(line, '\n'))
			break ;
	}
	free(line);
	return (*buffer);
}

char	*gt_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	if (!buffer[i])
		return (NULL);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	line = get_calloc(i + 2, sizeof(char));
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n' && buffer[i])
		line[i++] = '\n';
	return (line);
}

char	*get_next(char *buffer)
{
	int		i;
	int		j;
	char	*next;

	i = 0;
	j = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	next = get_calloc((get_strlen(buffer) - i + 1), sizeof(char));
	i++;
	while (buffer[i])
	{
		next[j] = buffer[i];
		j++;
		i++;
	}
	free(buffer);
	return (next);
}

/*
int	main(int argc, char **argv)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
}
*/
