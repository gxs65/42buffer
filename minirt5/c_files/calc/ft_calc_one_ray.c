#include "../../h_files/ft_minirt.h"

// Loops through objects to determine wether one of them intersects the ray,
// then if there was an intersection determines the color it produces
// 		/!\ disregards intersection with negative <t>,
// 			because it means the object is BEHIND the camera
t_vec	ft_draw_one_ray(t_data *data, t_ray ray)
{
	double		t_min;
	double		t_current;
	t_interval	hit_interval;
	t_object	*object_current;
	t_object	*object_closest;

	if (g_log)
	{
		printf("Calculating for new ray, recurr_level = %d\n", data->recurr_lvl);
		ft_ray_display(ray, 1);
	}
	if (data->recurr_lvl > MAX_RECURR_LVL)
		return (ft_vec_init(0, 0, 0));
	hit_interval = ft_interval_init(0.01, INFINITE_DOUBLE);
	t_min = INFINITE_DOUBLE;
	object_current = data->objects;
	object_closest = NULL;
	while (object_current)
	{
		t_current = ft_intersect_ray_object(ray, object_current, &hit_interval);
		if (t_current != -1)
		{
			t_min = t_current;
			object_closest = object_current;
			hit_interval.max = t_min;
		}
		object_current = object_current->next;
	}
	if (g_log)
	{
		if (object_closest)
			printf("\tRay intersected object %p at t = %f, calculating color\n", object_closest, t_min);
		else
			printf("\tRay did not intersect anything\n");
	}
	if (object_closest && object_closest->type == -1)
		return (ft_vec_init(0,0,0));
	else
		return (ft_color_ray_at(data, object_closest, ray, t_min));
}
