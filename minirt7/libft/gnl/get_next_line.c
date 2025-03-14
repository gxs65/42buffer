/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 20:24:07 by alex              #+#    #+#             */
/*   Updated: 2024/06/13 13:47:43 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*extract_end_of_line(char *tmp)
{
	char	*end_of_line;
	int		i;
	int		len;

	i = ft_line_len(tmp);
	len = ft_strlen(tmp) - i;
	if (!tmp[i])
		return (NULL);
	end_of_line = malloc(sizeof(char) * (len + 1));
	if (!end_of_line)
		return (NULL);
	len = i;
	i = -1;
	while (tmp[len + (++i)])
		end_of_line[i] = tmp[len + i];
	end_of_line[i] = '\0';
	return (end_of_line);
}

char	*extract_line(char *tmp)
{
	char	*line;
	int		i;
	int		len;

	i = 0;
	len = ft_line_len(tmp);
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (tmp[i] && tmp[i] != '\n')
	{
		line[i] = tmp[i];
		i++;
	}
	if (tmp[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*clear_tmp(char **tmp)
{
	free(*tmp);
	*tmp = NULL;
	return (NULL);
}

char	*read_line(int fd, char *tmp)
{
	char	*buff;
	int		i;
	char	*ret;

	i = 1;
	buff = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buff)
		return (clear_tmp(&tmp));
	while (!is_line(tmp) && i)
	{
		i = read(fd, buff, BUFFER_SIZE);
		if (i == -1)
		{
			free(buff);
			return (clear_tmp(&tmp));
		}
		buff[i] = '\0';
		ret = ft_strjoin_gnl(tmp, buff);
		free(tmp);
		tmp = ret;
	}
	free(buff);
	return (tmp);
}

char	*get_next_line(int fd)
{
	static char	*tmp;
	char		*line;
	char		*ret;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	tmp = read_line(fd, tmp);
	if (!tmp)
		return (NULL);
	if (!(*tmp))
		return (clear_tmp(&tmp));
	line = extract_line(tmp);
	if (!line)
		return (clear_tmp(&tmp));
	ret = extract_end_of_line(tmp);
	free(tmp);
	tmp = ret;
	return (line);
}

/*
int	main(void)
{
	int		fd;
	char	*line;
	clock_t	start;
	clock_t	end;
	double	cpu_time_used;

	fd = open("test.txt", O_RDONLY);
	start = clock();
	line = get_next_line(fd);
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Execution time: %f seconds\n", cpu_time_used);
	printf("line 1: %s\n", line);
	line = get_next_line(fd);
	printf("line 2: %s\n", line);
	line = get_next_line(fd);
	printf("line 3: %s\n", line);
	line = get_next_line(fd);
	printf("line 4: %s\n", line);
	line = get_next_line(fd);
	printf("line 5: %s\n", line);
	close(fd);
	return (0);
}
*/