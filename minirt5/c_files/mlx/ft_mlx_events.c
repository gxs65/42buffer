#include "../../h_files/ft_minirt.h"

// Functions hooking into MLX events

// Mouse events
int	ft_handle_mouse(int button, int x, int y, t_data *data)
{
	(void)button;

	ft_printf(LOGS, "mouse position : %d %d\n", x, y);
	ft_calc_sample_one_pixel(data, x, y);
	return (0);
}

// Applies a key event
// 		- key [Esc] : close window
// 		- key [u] : display log on window (useless)
// 		- key [i] : back to default parameters
// 		- keys [z], [x], [c], [a] : choice of plotting algo
// 		- keys [v], [b], [k], [l] : choice of coloring scheme
// 		- keys [up], [down], [left], [right] : move view
// 		  by a number of units proportionale to the current scale
// 		  (the more zoomed in, the less units of move)
// 		- keys [n], [m] : increase / decrease depth
int	ft_handle_keyrelease(int key, t_data *data)
{
	ft_printf(LOGS, ">>> Key release %i\n", key);
	(void)data;
	return (0);
}

// Closes the window
int	ft_handle_destroy(t_data *data)
{
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	return (0);
}
