#include <string>
#include <iostream>
#include "RPN.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	return (computeRPNExpression(std::string(av[1])));
}
