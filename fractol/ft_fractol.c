#include "ft_fractol.h"

int	main(int ac, char **av)
{
	void	*mlx;
	void	*mlx_win;
	t_img	img1;
	t_point	c;
	double	threshold;

	if (ac < 1)
		return (1);

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, WIN_WIDTH, WIN_HEIGHT, "Fractol");

	//new_image(mlx, &img1, 20, 20);
	new_image(mlx, &img1, WIN_WIDTH, WIN_HEIGHT);
	if (ac > 2)
	{
		c.x = ((double)atoi(av[1])) / 100;
		c.y = ((double)atoi(av[2])) / 100;
	}
	if (ac > 3)
		threshold = ((double)atoi(av[3])) / 100;
	else
		threshold = 2;
	//draw_julia(&img1, &c, threshold);
	draw_mandelbrot(&img1, threshold);
	(void)c;
	mlx_put_image_to_window(mlx, mlx_win, img1.img_ptr, 0, 0);

	ft_printf(LOGS, "Initialized mlx, created window, launching loop\n");
	mlx_loop(mlx);
	

	/*
	// TESTS FOR FUNCTION TIME TO DIVERGE
	(void)ac;
	(void)av;
	int		time;
	t_point	p;

	p.x = 1;
	p.y = 0.2;
	time = time_to_diverge(&p, -0.75, 5);
	*/
}