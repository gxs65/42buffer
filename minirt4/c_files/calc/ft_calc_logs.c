#include "../../h_files/ft_minirt.h"

void	ft_calc_log_camera(t_data *data)
{
	t_cam	*camera;

	camera = &(data->camera);
	printf("Camera def = pos, dvec, hvec, vvec (+ angle : %f) :\n", camera->fov_angle);
	ft_vec_display(camera->pos, 1);
	ft_vec_display(camera->dvec, 1);
	ft_vec_display(camera->hvec, 1);
	ft_vec_display(camera->vvec, 1);

	printf("Viewport def = width %f (pixel : %f), height %f (pixel : %f), center :\n",
		camera->vport_width, camera->vport_pix_width, camera->vport_height, camera->vport_pix_height);
	ft_vec_display(camera->vport_center, 1);

	printf("Viewport edge points = UpLeft, UpRight, DownLeft, DownRight :\n");
	ft_vec_display(camera->vport_ul, 1);
	ft_vec_display(camera->vport_ur, 1);
	ft_vec_display(camera->vport_dl, 1);
	ft_vec_display(camera->vport_dr, 1);
}

void	ft_calc_log_ray(t_ray ray, t_vec viewport_point, int pixel_x, int pixel_y)
{
	printf("Pixel %d'%d position on viewport : ", pixel_x, pixel_y);
	ft_vec_display(viewport_point, 0);
	printf(" -> ray : ");
	ft_ray_display(ray, 1);
}