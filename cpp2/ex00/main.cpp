#include "Fixed.hpp"

int		main(int ac, char **av)
{
	Fixed	a;
	Fixed	b(a);
	Fixed	c;

	(void)ac;
	(void)av;
	c = b;
	std::cout << a.getRawBits() << std::endl;
	std::cout << b.getRawBits() << std::endl;
	std::cout << c.getRawBits() << std::endl;
	return 0;
}
