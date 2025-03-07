#include "../../h_files/ft_minirt.h"

int	g_log;
int	g_count;

int	ft_handle_loop(t_data *data)
{
	(void)data;
	return (0);
}

void	ft_free_mlx_base(t_data *data)
{
	data->win_ptr = NULL;
	if (data->mlx_ptr)
	{
		mlx_destroy_display(data->mlx_ptr);
		free(data->mlx_ptr);
	}
	data->mlx_ptr = NULL;
}

void	ft_draw(t_data *data)
{
	ft_printf(LOGS, "Drawing...\n");
	ft_calc_rt(data);
	ft_printf(LOGS, "Displaying...\n");
	ft_display_changed_image(data, data->img);
	g_log = 1;
}

int	ft_init_parsing(t_data *data, char **av)
{
	data->alightning.unique_object_set = 0;
	data->lamp.unique_object_set = 0;
	data->camera.unique_object_set = 0;
	data->objects = NULL;
	if (ft_parse_scene(av[1], data))
		return (1);
	ft_log_scene(data);
	data->nb_samples_per_pixel = 25;
	data->recurr_lvl = 0;
	return (0);
}

int	ft_init_mlx(t_data *data, t_img *img)
{
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (1);
	data->win_ptr = mlx_new_window(data->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "MiniRT");
	if (!data->win_ptr)
	{
		mlx_destroy_display(data->mlx_ptr);
		free(data->mlx_ptr);
		return (1);
	}
	if (ft_new_image(data, img, WIN_WIDTH, WIN_HEIGHT))
	{
		ft_free_mlx_base(data);
		return (1);
	}
	ft_printf(LOGS, "MLX allocation successful\n");
	data->win_ratio = (double)WIN_HEIGHT / (double)WIN_WIDTH;
	mlx_loop_hook(data->mlx_ptr, &ft_handle_loop, data);
	mlx_key_hook(data->win_ptr, &ft_handle_keyrelease, data);
	mlx_mouse_hook(data->win_ptr, &ft_handle_mouse, data);
	mlx_hook(data->win_ptr, 33, 0, &ft_handle_destroy, data);
	return (0);
}

int	main(int ac, char **av)
{
	t_data		data;
	t_img		img;

	(void)ac;
	(void)av;
	if (ac != 2)
		return (1);
	data.img = &img;
	img.data = &data;
	g_log = 0;
	g_count = 0;
	if (ft_init_parsing(&data, av))
		return (1);
	if (ft_init_mlx(&data, &img))
		return (1);

	//cy();
	ft_draw(&data);
	mlx_loop(data.mlx_ptr);
	ft_free_mlx_base(&data);
	return (0);
}
