#ifndef __EASYFIND_H__
# define __EASYFIND_H__

# include <algorithm>

// SUMMARY OF STL CONTAINERS AVAILABLE IN C++98 (C++11)
// 		\ sequence containers : list, vector, deque, (array), (forward list)
// 		\ associative containers : set, multiset, map, multimap, (the same ones, unordered)
// 		\ container adapters : stack, queue, priority queue
// -> function <easyFind> manages only list/vector/deque,
// 	  since the subject excludes associative containers, and container adapters have no iterators


template <template<class, class> class C, class T, class A>
typename C<T,A>::iterator	easyFind(C<T,A>& container, T value)
{
	typename C<T,A>::iterator	it;

	it = std::find(container.begin(), container.end(), value);
	if (it == container.end())
		std::cout << "value " << value << " not found\n";
	else
		std::cout << "value " << value << " found as " << *it << "\n";
	return (it);
}

template <template<class, class> class C, class T, class A>
void	displayContainer(C<T, A >& container)
{
	int							ind = 0;
	typename C<T,A>::iterator	it;

	for (it = container.begin(); it != container.end(); it++)
	{
		std::cout << "pos " << ind << " : " << *it << "\n";
		ind++;
	}
}

/*
template <template<class, class> class C, class T, template <class> class A>
void	displayContainer(C<T, A<T> >& container)
{
	int		ind;
	T		elem;

	for (ind = 0; ind < 3; ind++)
	{
		elem = container[ind];
		std::cout << "pos " << ind << " : " << elem << "\n";
	}
}
*/

/*
template <template <class> class C, class T>
void	displayContainer(C<T>& container)
{
	int		ind;

	for (ind = 0; ind < 3; ind++)
	{
		std::cout << "pos " << ind << " : " << container[ind] << "\n";
	}
}

template <class C>
void	displayContainer(C& container)
{
	int		ind;

	for (ind = 0; ind < 3; ind++)
	{
		std::cout << "pos " << ind << " : " << container[ind] << "\n";
	}
}
*/

#endif
