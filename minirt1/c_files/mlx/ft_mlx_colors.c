#include "../../h_files/ft_minirt.h"

// Creates a single 4-bytes integer (int)
// out of the 4 intensities (0-255) of ARGB components
int	ft_assemble_color(int a, int r, int g, int b)
{
	int	color;

	color = (a * 0x01000000 + r * 0x010000 + g * 0x0100 + b * 0x01);
	return (color);
}
