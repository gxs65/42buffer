#include "../../h_files/ft_minirt.h"

// Defines steps to be executed at each MLX llop cycle :
// 		here, nothing (every action is triggered by user event)
int	ft_handle_loop(t_mlxdata *data)
{
	(void)data;
	return (0);
}

// Frees base objects from MLX : display and window
// (does NOT call <mlx_destroy_window>,
//  since it must have been called by the function
//  that treated the program-ending function)
void	ft_free_mlx_base(t_mlxdata *data)
{
	data->win_ptr = NULL;
	if (data->mlx_ptr)
	{
		mlx_destroy_display(data->mlx_ptr);
		free(data->mlx_ptr);
	}
	data->mlx_ptr = NULL;
}

void	ft_draw_gradient(t_mlxdata *data)
{
	int		x;
	int		y;
	double	r;
	double	g;
	int		color;

	y = 0;
	while (y < data->img->height)
	{
		x = 0;
		while (x < data->img->width)
		{
			r = (double)x / data->img->width;
			g = (double)y / data->img->height;
			color = ft_assemble_color(0, (int)(255 * r), (int)(255 * g), 0);
			ft_change_pixel(data->img, x, y, color);
			x++;
		}
		y++;
	}
}

void	ft_draw(t_mlxdata *data)
{
	ft_printf(LOGS, "Drawing...\n");
	ft_draw_gradient(data);
	ft_printf(LOGS, "Displaying...\n");
	ft_display_changed_image(data, data->img);
	ft_calc_rt(data);
}

// Initialization of the fractol window
// 		- MLX initialization
// 		- creation of <t_img> instance, allocation of its <times> table
// 			initially containing -2 for all pixels (<=> "never explored")
// 		- parsing parameters
// 		- creating MLX hooks for all events
// 		- drawing initial image (and side log)
// 		- launching MLX loop
int	main(int ac, char **av)
{
	t_mlxdata	data;
	t_img		img;

	(void)ac;
	(void)av;
	data.img = &img;
	img.data = &data;
	
	data.mlx_ptr = mlx_init();
	if (!data.mlx_ptr)
		return (1);
	data.win_ptr = mlx_new_window(data.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "MiniRT");
	if (!data.win_ptr)
	{
		mlx_destroy_display(data.mlx_ptr);
		free(data.mlx_ptr);
		return (1);
	}
	if (ft_new_image(&data, &img, WIN_WIDTH, WIN_HEIGHT))
	{
		ft_free_mlx_base(&data);
		return (1);
	}
	ft_printf(LOGS, "Allocation successful, arguments read\n");

	data.win_ratio = (double)WIN_HEIGHT / (double)WIN_WIDTH;
	mlx_loop_hook(data.mlx_ptr, &ft_handle_loop, &data);
	mlx_key_hook(data.win_ptr, &ft_handle_keyrelease, &data);
	mlx_mouse_hook(data.win_ptr, &ft_handle_mouse, &data);
	mlx_hook(data.win_ptr, 33, 0, &ft_handle_destroy, &data);

	ft_draw(&data);
	mlx_loop(data.mlx_ptr);
	ft_free_mlx_base(&data);
}
