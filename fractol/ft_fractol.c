#include "ft_fractol.h"

int	handle_loop(t_mlxdata *data)
{
	(void)data;
	return (0);
}

void	draw(t_mlxdata *data)
{
	draw_mandelbrot3(data->img);
	display_changed_image(data, data->img);
}

int	main(int ac, char **av)
{
	t_mlxdata	data;
	t_img		img;

	data.mlx_ptr = mlx_init();
	data.win_ptr = mlx_new_window(data.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "Fractol");

	//new_image(data.mlx_ptr, &img, WIN_WIDTH, WIN_HEIGHT);
	new_image(data.mlx_ptr, &img, WIN_WIDTH, WIN_HEIGHT);
	img.ftype = 1;// TODO : update to allow Julia sets
	if (img.ftype == 0)
	{
		img.c.x = ((double)atoi(av[1])) / 100;
		img.c.y = ((double)atoi(av[2])) / 100;
	}
	if (ac == -1) // TODO : update to allow threshold parameter
	{
		img.threshold_sq = ((double)atoi(av[3])) / 100;
		img.threshold_sq = img.threshold_sq * img.threshold_sq;
	}
	else
		img.threshold_sq = 2 * 2;

	data.img = &img;
	img.data = &data;
	mlx_loop_hook(data.mlx_ptr, &handle_loop, &data);
	mlx_key_hook(data.win_ptr, &handle_keyrelease, &data);
	mlx_mouse_hook(data.win_ptr, &handle_mouse, &data);
	mlx_hook(data.win_ptr, 17, 0, &handle_destroy, &data);

	draw(&data);
	mlx_loop(data.mlx_ptr);
    mlx_destroy_display(data.mlx_ptr);
}