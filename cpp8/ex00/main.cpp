#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include "easyFind.hpp"

int	main(void)
{
	std::vector<int>									v;
	std::list<int>										l;
	std::deque<int>										d;
	std::vector<int>::iterator							itv;
	std::list<int>::iterator							itl;
	std::deque<int>::iterator							itd;

	v.push_back(6);
	v.push_back(12);
	v.push_back(24);
	l.push_back(3);
	l.push_back(4);
	l.push_back(5);
	d.push_back(20);
	d.push_back(30);
	d.push_back(40);
	(void)itv;
	(void)itl;

	/*
	std::cout << "at pos 1 : " << v[1] << "\n";
	it = std::find(v.begin(), v.end(), 12);
	std::cout << "result of find = pos " << (it - v.begin()) << ", content " << *it << "\n";
	v[1] = 8;
	std::cout << "result of find = pos " << (it - v.begin()) << ", content " << *it << "\n";
	*/

	
	displayContainer(v);
	std::cout << "at pos 1 : " << v[1] << "\n";
	itv = easyFind(v, 12);
	std::cout << "result of find = pos " << (itv - v.begin()) << ", content " << *itv << "\n";
	v[1] = 8;
	std::cout << "result of find = pos " << (itv - v.begin()) << ", content " << *itv << "\n";
	
	displayContainer(l);
	std::cout << "at pos 0 : " << l.front() << "\n";
	itl = easyFind(l, 3);
	std::cout << "result of find - content " << *itl << "\n";
	l.front() = 1;
	std::cout << "result of find - content " << *itl << "\n";

	displayContainer(d);
	std::cout << "at pos 0 : " << d[2] << "\n";
	itd = easyFind(d, 40);
	std::cout << "result of find - content " << *itd << "\n";
	d[2] = 100;
	std::cout << "result of find - content " << *itd << "\n";


	return 0;
}