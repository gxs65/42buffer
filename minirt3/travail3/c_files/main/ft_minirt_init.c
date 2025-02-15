#include "../../h_files/ft_minirt.h"

void	ft_log_objects(t_data *data)
{
	int			ind;
	t_object	*object;

	ind = 0;
	while (data->objects[ind].type != -1)
	{
		object = &(data->objects[ind]);
		printf("Object %d : type %d, dist %f, material %d ; start + albedo : ",
			ind, object->type, object->dist, object->material);
		ft_vec_display(object->start, 1);
		ft_vec_display(object->albedo, 1);
		ind++;
	}
}

int	ft_load_objects(t_data *data)
{
	data->objects = malloc((3 + 1) * sizeof(t_object));
	if (!data->objects)
		return (1);
	data->objects[3].type = -1;

	data->objects[0].type = SPHERE;
	data->objects[0].dist = 1;
	data->objects[0].start = ft_vec_init(0, 2, 1);
	data->objects[0].albedo = ft_vec_init(0.6, 0.6, 0.999);
	data->objects[0].material = LAMBERTIAN;

	data->objects[1].type = SPHERE;
	data->objects[1].dist = 0.5;
	data->objects[1].start = ft_vec_init(2, 2, 1);
	data->objects[1].albedo = ft_vec_init(0.7, 0.7, 0.7);
	data->objects[1].material = METAL;

	data->objects[2].type = SPHERE;
	data->objects[2].dist = 10;
	data->objects[2].start = ft_vec_init(0, 2, -10);
	data->objects[2].albedo = ft_vec_init(0.999, 0.1, 0.1);
	data->objects[2].material = LAMBERTIAN;

	ft_log_objects(data);
	return (0);
}
