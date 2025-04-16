#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "quicksort.hpp"

bool	isValidNumber(std::string token)
{
	(void)token;
	return (1);
}

bool	isSorted(std::vector<int> v)
{
	std::vector<int>::iterator	it;

	for (it = v.begin(); it != v.end(); it++)
	{
		if (it != v.begin() && *it < *(it - 1))
			return (0);
	}
	return (1);
}

void	strToIntArray(std::vector<int>& v, std::string s)
{
	std::stringstream	parserStream;
	std::string			token;

	parserStream << s;
	while (std::getline(parserStream, token, ' '))
	{
		if (isValidNumber(token))
			v.push_back(static_cast<int>(strtol(token.c_str(), NULL, 10)));
	}
}

void	argsToIntArray(std::vector<int>& v, char **av)
{
	int	ind = 0;

	while (av[ind])
	{
		if (isValidNumber(std::string(av[ind])))
			v.push_back(static_cast<int>(strtol(av[ind], NULL, 10)));
		ind++;
	}
}

int	main(int ac, char **av)
{
	std::vector<int>	v;

	if (ac < 2)
		return (1);
	else if (ac == 2)
		strToIntArray(v, std::string(av[1]));
	else
		argsToIntArray(v, av + 1);
	std::cout << "Before sorting :\n";
	logVector(v);
	quickSort(v, 0, v.size() - 1);
	std::cout << "After sorting :\n";
	logVector(v);
	std::cout << "Is sorted : " << isSorted(v) << "\n";

	return (0);
}