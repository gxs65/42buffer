#include "ft_fractol.h"

// Functions hooking into MLX events

// Mouse events :
// 		[mouse scrollwheel] -> applies a zoom while preserving
// 							   the cursor's position on the fractal
// 		- <mousepos_ratios> is not a point, but the relative cursor position
// 		- <ref_point> is the coordinates of the point of the COMPLEX plane
// 		  on which the cursor currently lies
int	handle_mouse(int button, int x, int y, t_mlxdata *data)
{
	t_point	mousepos_ratios;
	t_point	ref_point;

	ft_printf(LOGS, ">>> Received mouse event at %d'%d : button %d\n", x, y, button);
	if (button == Button5 || button == Button4)
	{
		mousepos_ratios.x = (double)x / (double)data->img->width;
		mousepos_ratios.y = (double)y / (double)data->img->height;
		ref_point.x = data->img->nw_coords.x + (mousepos_ratios.x * data->img->scale);
		ref_point.y = data->img->nw_coords.y + (mousepos_ratios.y * data->img->scale);
		printf("\t-> zooming in/out preserving ref point %.10f'%.10f\n", ref_point.x, ref_point.y); // LOG WITH PRINTF
		if (button == Button5)
			data->img->scale = 1.1 * data->img->scale;
		else
			data->img->scale = 0.9 * data->img->scale;
		if (data->img->scale > 4)
			data->img->scale = 4;
		printf("\t-> modified scale, value now : %.10f\n", data->img->scale); // LOG WITH PRINTF
		data->img->nw_coords.x = ref_point.x - (mousepos_ratios.x * data->img->scale);
		data->img->nw_coords.y = ref_point.y - (mousepos_ratios.y * data->img->scale * (data->img->height / data->img->width));
		draw(data);
	}
	return (0);
}

// Applies a key event
// 		- key [Esc] : close window
// 		- key [a] : redraw the set entirely
// 		- keys [up], [down], [left], [right] : move view
// 		  by a number of units proportionale to the current scale
// 		  (the more zoomed in, the less units of move)
// 		- keys [n], [m] : increase / decrease depth
int	handle_keyrelease(int keysym, t_mlxdata *data)
{
	ft_printf(LOGS, ">>> Received key release %i\n", keysym);
	if (keysym == XK_Escape)
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	if (keysym == XK_a)
	{
		draw(data);
	}
	if (keysym == XK_Left || keysym == XK_Right
		|| keysym == XK_Down || keysym == XK_Up)
	{
		if (keysym == XK_Left)
			data->img->nw_coords.x -= (data->img->scale) / 8;
		if (keysym == XK_Right)
			data->img->nw_coords.x += (data->img->scale) / 8;
		if (keysym == XK_Down)
			data->img->nw_coords.y += (data->img->scale) / 8;
		if (keysym == XK_Up)
			data->img->nw_coords.y -= (data->img->scale) / 8;
		printf("\t-> coords of view changed, NW now : %.10f'%.10f\n", data->img->nw_coords.x, data->img->nw_coords.y); // LOG WITH PRINTF
		draw(data);
	}
	if (keysym == XK_n || keysym == XK_m)
	{
		if (keysym == XK_n)
			data->img->depth += data->img->depth / 10 + (data->img->depth < 10);
		if (keysym == XK_m)
			data->img->depth -= data->img->depth / 10;
		ft_printf(LOGS, "\t-> modified depth, value now : %d\n", data->img->depth);
		draw(data);
	}
	return (0);
}

// Closes the window
int	handle_destroy(t_mlxdata *data)
{
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	return (0);
}