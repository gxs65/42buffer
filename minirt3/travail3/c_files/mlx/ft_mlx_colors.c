#include "../../h_files/ft_minirt.h"

// Creates a single 4-bytes integer (int)
// out of the 4 intensities (0-255) of ARGB components
int	ft_assemble_color(t_vec color_components)
{
	int	color;
	int	a;
	int	r;
	int	g;
	int	b;

	a = 0;
	
	r = (int)(255 * sqrt(ft_fabs(color_components.x)));
	g = (int)(255 * sqrt(ft_fabs(color_components.y)));
	b = (int)(255 * sqrt(ft_fabs(color_components.z)));
	
	/*
	r = (int)(255 * (color_components.x));
	g = (int)(255 * (color_components.y));
	b = (int)(255 * (color_components.z));
	*/
	color = (a * 0x01000000 + r * 0x010000 + g * 0x0100 + b * 0x01);
	return (color);
}
