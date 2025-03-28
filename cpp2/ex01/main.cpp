#include "Fixed.hpp"

void	display_char_hexa(unsigned char c)
{
	int	d = (int)(c / 16);
	int	u = (int)(c % 16);
	//std::cout << "n " << (int)c << " d " << d << " u " << u;
	if (d > 9)
		std::cout << (unsigned char)('a' + (d - 10));
	else
		std::cout << d;
	if (u > 9)
		std::cout << (unsigned char)('a' + (u - 10));
	else
		std::cout << u;
}

void	display_int_raw(int n)
{
	int	ind;

	std::cout << "Raw bits of integer " << n << " : ";
	for(ind = 0; ind < 32; ind++)
	{
		std::cout << ((n >> (31 - ind)) & 1) << " ";
	}
	std::cout << "\n";
}

void	display_float_raw(float b)
{
	unsigned char 	*b_raw;
	unsigned int	ind;

	b_raw = reinterpret_cast<unsigned char *>(&b);
	std::cout << "Bit representation of float " << b << " :\n";
	std::cout << "[BYTES]\n";
	for (ind = 0; ind < 4; ind++)
	{
		display_char_hexa(b_raw[3 - ind]);
		std::cout << " ; ";
	}

	std::cout << "\n[BITS]\n";
	std::cout << "\nSign bit : " << ((b_raw[3] >> 7) & 1) << "\nExponent : ";
	for (ind = 1; ind < 8; ind++)
		std::cout << ((b_raw[3] >> (7 - ind)) & 1) << " ";
	std::cout << (b_raw[2] >> 7 & 1) << "\nMantissa : ";
	for (ind = 0; ind < 24; ind++)
	{
		if (ind != 0)
			std::cout << ((b_raw[2 - (ind / 8)] >> (7 - (ind % 8))) & 1) << " ";
	}
	std::cout << "\n";
}

int		bitarray_to_int(unsigned char *arr, int size)
{
	int	n;
	int	ind;

	n = 0;
	ind = 0;
	while (ind < size)
	{
		n = n * 2 + (int)(arr[ind]);
		ind++;
	}
	return (n);
}

void	fill_bitarray(unsigned char *arr, unsigned char filler, int size)
{
	int	ind;

	ind = 0;
	while (ind < size)
	{
		arr[ind] = filler;
		ind++;
	}
}

int		main(int ac, char **av)
{
	(void)ac;
	(void)av;

	
	Fixed	a((float)atof(av[1]));
	//a.convertFromFloat(atof(av[1]));
	//a.convertFromInt(atoi(av[1]));
	

	/*
	display_int_raw(atoi(av[1]));
	*/

	return 0;
}
