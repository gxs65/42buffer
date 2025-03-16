/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:13:09 by abedin            #+#    #+#             */
/*   Updated: 2025/03/14 19:06:37 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	ft_register_line(char *line, t_data *data)
{
	char	**words;
	int		nb_words;

	words = ft_split(line, ' ');
	if (!words)
		return (1);
	nb_words = 0;
	while (words[nb_words])
		nb_words++;
	if (nb_words == 0 || (nb_words > 0 && ft_strlen(words[0]) > 0 &&
		(words[0][0] == '#' || words[0][0] == '\n')))
	{
		//printf("Empty line or commented line\n");
		return (0);
	}
	else
	{
		printf("PARSER on line ::: %s", line);
		if (ft_strcmp(words[0], "A") == 0)
			return (ft_register_alightning(words, nb_words, data));
		else if (ft_strcmp(words[0], "L") == 0)
			return (ft_register_lamp(words, nb_words, data));
		else if (ft_strcmp(words[0], "C") == 0)
			return (ft_register_camera(words, nb_words, data));
		else if (ft_strcmp(words[0], "pl") == 0)
			return (ft_register_plane(words, nb_words, data));
		else if (ft_strcmp(words[0], "sp") == 0)
			return (ft_register_sphere(words, nb_words, data));
		else if (ft_strcmp(words[0], "cy") == 0)
			return (ft_register_cylinder(words, nb_words, data));
		else if (ft_strcmp(words[0], "co") == 0)
			return (ft_register_cone(words, nb_words, data));
		else
			return (1);
	}
}

int	ft_parse_scene(char *scene_file, t_data *data)
{
	int		fd;
	char	*line;
	int		retval;

	fd = open(scene_file, O_RDONLY);
	if (fd < 0)
		return (1);
	while (fd)
	{
		line = get_next_line(fd);
		if (!line)
		{
			printf("End of scene description file\n");
			close(fd);
			fd = 0;
		}
		else
		{
			retval = ft_register_line(line, data);
			if (retval)
			{
				printf("Error when registering a line : error %d\n", retval);
				free(line);
				close(fd);
				return (1);
			}
			free(line);
		}
	}
	if (!data->camera.unique_object_set || !data->alightning.unique_object_set
		|| !(data->lights))
	{
		printf("Scene lacks a mandatory element (camera, lamp, ambient lightning)");
		return (1);
	}
	printf("Scene successfully parsed\n");
	return (0);
}
