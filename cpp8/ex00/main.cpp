#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "easyFind.hpp"

int	main(void)
{
	std::vector<int>			v;
	std::vector<int>::iterator	it;
	int*						location;

	v.push_back(6);
	v.push_back(12);
	v.push_back(24);

	/*
	std::cout << "at pos 1 : " << v[1] << "\n";
	it = std::find(v.begin(), v.end(), 12);
	std::cout << "result of find = pos " << (it - v.begin()) << ", content " << *it << "\n";
	v[1] = 8;
	std::cout << "result of find = pos " << (it - v.begin()) << ", content " << *it << "\n";
	*/

	std::cout << "at pos 1 : " << v[1] << "\n";
	location = easyFind(v, 12);
	std::cout << "result of find = pos " << (location) << ", content " << *location << "\n";
	v[1] = 8;
	std::cout << "result of find = pos " << (location) << ", content " << *location << "\n";

	return 0;
}