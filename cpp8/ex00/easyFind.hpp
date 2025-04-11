#ifndef __EASYFIND_H__
# define __EASYFIND_H__

# include <algorithm>

// SUMMARY OF STL CONTAINERS AVAILABLE IN C++98 (C++11)
// 		\ sequence containers : list, vector, deque, (array), (forward list)
// 		\ associative containers : set, multiset, map, multimap, (the same ones, unordered)
// 		\ container adapters : stack, queue, priority queue
// -> function <easyFind> manages only list/vecto/deque,
// 	  since the subject excludes associative containers, and container adapters have no iterators

template <class TContainer>
int*	easyFind(TContainer container, int value)
{
	int*	location;

	location = reinterpret_cast<int *>(std::find(container.begin(), container.end(), value));
	return (location);
}

#endif
