#include "../../h_files/ft_minirt.h"

typedef struct s_camera
{
	t_vec	pos;
	t_vec	dvec;
	t_vec	hvec;
	t_vec	vvec;
	double	fov_angle;

	t_vec	vport_center;
	double	vport_width;
	double	vport_height;
	t_vec	vport_ul;
	t_vec	vport_ur;
	t_vec	vport_dl;
	t_vec	vport_dr;



}				t_cam;

void	ft_log_camera(t_cam *camera)
{
	printf("Camera def = pos, dvec, hvec, vvec (+ angle : %f) :\n", camera->fov_angle);
	ft_vec_display(camera->pos, 1);
	ft_vec_display(camera->dvec, 1);
	ft_vec_display(camera->hvec, 1);
	ft_vec_display(camera->vvec, 1);

	printf("Viewport def = width %f, height %f, center :\n", camera->vport_width, camera->vport_height);
	ft_vec_display(camera->vport_center, 1);

	printf("Viewport edge points = UpLeft, UpRight, DownLeft, DownRight :\n");
	ft_vec_display(camera->vport_ul, 1);
	ft_vec_display(camera->vport_ur, 1);
	ft_vec_display(camera->vport_dl, 1);
	ft_vec_display(camera->vport_dr, 1);


}

void	ft_calc_rt(t_mlxdata *data)
{
	t_cam	camera;

	camera.pos = ft_vec_init(1, 1, 0);
	camera.dvec = ft_vec_setnorm(ft_vec_init(5, 9, 4), 1.0);
	camera.hvec = ft_vec_setnorm(ft_vec_init(8, -4, -1), 1.0);
	camera.vvec = ft_vec_setnorm(ft_vec_vproduct(camera.dvec, camera.hvec), 1.0);

	camera.fov_angle = 30;

	camera.vport_center = ft_vec_add(camera.pos, camera.dvec);
	camera.vport_width = 2 * tan(camera.fov_angle / 2);
	camera.vport_height = camera.vport_width * data->win_ratio;

	camera.vport_ul = 	ft_vec_add(camera.vport_center,
						ft_vec_add(	ft_vec_scale(camera.vvec, -1 * camera.vport_height / 2),
									ft_vec_scale(camera.hvec, -1 * camera.vport_width / 2)));
	camera.vport_ur = 	ft_vec_add(camera.vport_center,
						ft_vec_add(	ft_vec_scale(camera.vvec, -1 * camera.vport_height / 2),
									ft_vec_scale(camera.hvec, 1 * camera.vport_width / 2)));
	camera.vport_dl = 	ft_vec_add(camera.vport_center,
						ft_vec_add(	ft_vec_scale(camera.vvec, 1 * camera.vport_height / 2),
									ft_vec_scale(camera.hvec, -1 * camera.vport_width / 2)));
	camera.vport_dr = 	ft_vec_add(camera.vport_center,
						ft_vec_add(	ft_vec_scale(camera.vvec, 1 * camera.vport_height / 2),
									ft_vec_scale(camera.hvec, 1 * camera.vport_width / 2)));

	ft_log_camera(&camera);
}