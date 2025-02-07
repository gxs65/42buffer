#include "../../h_files/ft_minirt.h"

// Loops through objects to determine wether one of them intersects the ray,
// then if there was an intersection determines the color it produces
// 		/!\ disregards intersection with negative <t>,
// 			because it means the object is BEHIND the camera
t_vec	ft_draw_one_ray(t_data *data, t_ray ray)
{
	int			ind_min;
	int			ind_current;
	double		t_min;
	double		t_current;
	t_interval	hit_interval;

	if (data->recurr_lvl > 10)
		return (ft_vec_init(0, 0, 0));
	//printf("\tNew ray launched, recurr_lvl = %d\n", data->recurr_lvl);
	hit_interval = ft_interval_init(0.01, INFINITE_DOUBLE);
	ind_current = 0;
	ind_min = -1;
	t_min = INFINITE_DOUBLE;
	while (data->objects[ind_current].type != -1)
	{
		t_current = ft_intersect_ray_object(ray, data->objects[ind_current], &hit_interval);
		if (t_current != -1)
		{
			t_min = t_current;
			ind_min = ind_current;
			hit_interval.max = t_min;
		}
		ind_current++;
	}
	//printf("%c", '0' + (ind_min != -1));
	return (ft_color_ray_at(data, ind_min, ray, t_min));
}
