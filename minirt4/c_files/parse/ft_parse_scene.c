#include "../../h_files/ft_minirt.h"

// ambient lightning, lamp, camera
// sphere, plane, cylinder

int	ft_register_alightning(char **words, int nb_words, t_data *data)
{
	printf("Registering ambient lightning\n");
	if (data->alightning.unique_object_set)
		return (3);
	if (nb_words != 3)
		return (1);
	data->alightning.dist = ft_atof(words[1]); // = ambient lightning intensity ?
	if (ft_vec_from_str(&(data->alightning.albedo), words[2])) // = ambient lightning color
		return (2);
	data->alightning.albedo = ft_vec_scale(data->alightning.albedo, 1.0 / 256.0);
	data->alightning.unique_object_set = 1;
	return (0);
}

int	ft_register_lamp(char **words, int nb_words, t_data *data)
{
	printf("Registering lamp\n");
	if (data->lamp.unique_object_set)
		return (3);
	if (nb_words != 4)
		return (1);
	if (ft_vec_from_str(&(data->lamp.start), words[1])) // = lamp position
		return (2);
	data->lamp.dist = ft_atof(words[2]); // = light brightness ?
	if (ft_vec_from_str(&(data->lamp.albedo), words[3])) // = lamp color
		return (2);
	data->lamp.albedo = ft_vec_scale(data->lamp.albedo, 1.0 / 256.0);
	data->lamp.unique_object_set = 1;
	return (0);
}

int	ft_register_camera(char **words, int nb_words, t_data *data)
{
	printf("Registering camera\n");
	if (data->camera.unique_object_set)
		return (3);
	if (nb_words != 5)
		return (1);
	if (ft_vec_from_str(&(data->camera.pos), words[1])) // = camera position
		return (2);
	if (ft_vec_from_str(&(data->camera.dvec), words[2])) // = camera direction vector
		return (2);
	if (ft_vec_from_str(&(data->camera.hvec), words[3])) // = camera horizontal vector (! not given in subject)
		return (2);
	data->camera.fov_angle = ft_atof(words[4]); // = camera horizontal field of view in degrees
	data->camera.unique_object_set = 1;
	return (0);
}

int	ft_register_sphere(char **words, int nb_words, t_data *data)
{
	t_object	*sphere;

	printf("Registering sphere\n");
	if (nb_words != 5)
		return (1);
	sphere = ft_object_new();
	if (!sphere)
		return (2);
	sphere->type = SPHERE;
	if (ft_vec_from_str(&(sphere->start), words[1])) // = sphere center
		return (2);
	sphere->dist = ft_atof(words[2]); // = sphere radius
	if (ft_vec_from_str(&(sphere->albedo), words[3])) // = sphere color
		return (2);
	sphere->albedo = ft_vec_scale(sphere->albedo, 1.0 / 256.0);
	sphere->material = ft_atoi(words[4]);
	ft_object_push_back(&(data->objects), sphere);
	return (0);
}

int	ft_register_plane(char **words, int nb_words, t_data *data)
{
	t_object	*plane;

	printf("Registering plane\n");
	if (nb_words != 5)
		return (1);
	plane = ft_object_new();
	if (!plane)
		return (2);
	plane->type = PLANE;
	if (ft_vec_from_str(&(plane->start), words[1])) // = one point on the plane
		return (2);
	if (ft_vec_from_str(&(plane->normal), words[2])) // = plane normal vector
		return (2);
	if (ft_vec_from_str(&(plane->albedo), words[3])) // = plane color
		return (2);
	plane->albedo = ft_vec_scale(plane->albedo, 1.0 / 256.0);
	plane->material = ft_atoi(words[4]);
	ft_object_push_back(&(data->objects), plane);
	return (0);
}

int	ft_register_cylinder(char **words, int nb_words, t_data *data)
{
	t_object	*cylinder;

	printf("Registering cylinder\n");
	if (nb_words != 7)
		return (1);
	cylinder = ft_object_new();
	if (!cylinder)
		return (2);
	cylinder->type = CYLINDER;
	if (ft_vec_from_str(&(cylinder->start), words[1])) // = center of cylinder
		return (2);
	if (ft_vec_from_str(&(cylinder->normal), words[2])) // = axis of cylinder
		return (2);
	cylinder->dist = ft_atof(words[3]);
	cylinder->height = ft_atof(words[4]);
	if (ft_vec_from_str(&(cylinder->albedo), words[5])) // = plane color
		return (2);
	cylinder->albedo = ft_vec_scale(cylinder->albedo, 1.0 / 256.0);
	cylinder->material = ft_atoi(words[6]);
	ft_object_push_back(&(data->objects), cylinder);
	return (0);
}

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
	if (nb_words == 0)
		return (0);
	else if (ft_strcmp(words[0], "A") == 0)
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
	else
		return (1);
}

int	ft_parse_scene(char *scene_file, t_data *data)
{
	int		fd;
	char	*line;
	int		ind;
	int		retval;

	fd = open(scene_file, O_RDONLY);
	if (fd < 0)
		return (1);
	ind = 0;
	while (1)
	{
		line = get_next_line(fd, 0);
		printf("PARSER on line : <<< %s >>>\n", line);
		if (!line)
		{
			printf("End of scene description file\n");
			close(fd);
			return (0);
		}
		retval = ft_register_line(line, data);
		if (retval)
		{
			printf("Error when registering line : %d\n", retval);
			free(line);
			get_next_line(fd, 1);
			close(fd);
			return (1);
		}
		free(line);
	}
	return (0);
}


void	ft_log_objects(t_object *current)
{
	if (current)
	{
		if (current->type == SPHERE)
		{
			printf("Object %p : SPHERE mat %d, radius %f, center + color :\n",
				current, current->material, current->dist);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->albedo, 1);
		}
		else if (current->type == PLANE)
		{
			printf("Object %p : PLANE mat %d, point + normal vector + color :\n",
				current, current->material);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->normal, 1);
			ft_vec_display(current->albedo, 1);
		}
		else if (current->type == CYLINDER)
		{
			printf("Object %p : CYLINDER mat %d, radius %f, height %f, center + axis + color :\n",
				current, current->material, current->dist, current->height);
			ft_vec_display(current->start, 1);
			ft_vec_display(current->normal, 1);
			ft_vec_display(current->albedo, 1);
		}
		ft_log_objects(current->next);
	}
}

void	ft_log_scene(t_data *data)
{
	printf("==========\n");
	printf("Camera angle = %f, pos + dvec :\n", data->camera.fov_angle);
	ft_vec_display(data->camera.pos, 1);
	ft_vec_display(data->camera.dvec, 1);
	if (data->lamp.unique_object_set)
	{
		printf("Lamp at intensity %f, position + color :\n", data->lamp.dist);
		ft_vec_display(data->lamp.start, 1);
		ft_vec_display(data->lamp.albedo, 1);
	}
	if (data->alightning.unique_object_set)
	{
		printf("Ambient lightning at intensity %f, colored :\n", data->alightning.dist);
		ft_vec_display(data->alightning.albedo, 1);
	}
	ft_log_objects(data->objects);
	printf("==========\n");
}
