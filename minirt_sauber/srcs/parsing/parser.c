/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abedin <abedin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 19:13:09 by abedin            #+#    #+#             */
/*   Updated: 2025/03/20 21:24:18 by abedin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Forwards the scene line to the adequate registering function
// according to the type of object described
int	ft_register_line_by_type(char **words, int nb_words, t_data *data)
{
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
	{
		printf("Object type is not recognized by parser\n");
		return (1);
	}
}

// Splits a scene line into words and checks that it is not an empty line
// or a commentary line (beginning with '#') 
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
	if (nb_words == 0 || (nb_words > 0 && ft_strlen(words[0]) > 0
			&& (words[0][0] == '#' || words[0][0] == '\n')))
		return (0);
	else
	{
		printf("PARSER on line ::: %s", line);
		return (ft_register_line_by_type(words, nb_words, data));
	}
}

// Checks if the <line> produced by <get_next_line> is NULL,
// which means the end of the scene file has been read
// If there is a line, sends it to <register_line> 
// and checks if it produces a parse error
int	ft_parse_scene_line(char *line, int fd, t_data *data)
{
	int	parse_error;

	if (!line)
	{
		printf("End of scene description file\n");
		close(fd);
		return (1);
	}
	parse_error = ft_register_line(line, data);
	free(line);
	if (parse_error)
	{
		printf("Error on a scene line : error %d\n", parse_error);
		close(fd);
		return (1);
	}
	return (0);
}

// Reads the scene file line by line using <get_next_line>,
// then checks if all mandatory elements are present
int	ft_parse_scene(char *scene_file, t_data *data)
{
	int		fd;
	char	*line;
	int		end_of_parsing;

	fd = open(scene_file, O_RDONLY);
	if (fd < 0)
		return (1);
	end_of_parsing = 0;
	while (!end_of_parsing)
	{
		line = get_next_line(fd);
		end_of_parsing = ft_parse_scene_line(line, fd, data);
	}
	if (!data->camera.unique_object_set || !data->alightning.unique_object_set
		|| !(data->lights))
	{
		printf("Scene lacks a mandatory element (camera, lamp, ambient)");
		return (1);
	}
	printf("Scene successfully parsed\n");
	return (0);
}

// Sets the <unique_object_set> flags to 0 before beginning
// the parsing of the scene described in the file given in <av[1]>
int	ft_init_parsing(t_data *data, char **av)
{
	data->alightning.unique_object_set = 0;
	data->lights = NULL;
	data->camera.unique_object_set = 0;
	data->objects = NULL;
	if (ft_parse_scene(av[1], data))
		return (1);
	data->nb_samples_per_pixel = 10;
	data->recurr_lvl = 0;
	return (0);
}
