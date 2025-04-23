#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>

// Result of tests : both wrapper methods
// 		(WrapperA : defines a template function and explicitly gives 2 of its specializations,
// 		 WrapperB : uses function overload to let compiler select the correct specialization)
// yield the same result ; method A is probably more confusing
// since it could make readers suppose that there is a 'general form' of template function <insertion>


class	WrapperA
{
	public:
		WrapperA() {};
		template <class C> void	insertion(C& c, int x);
		template <class C> void	display(C& c);
	private:
};
template <class C>
void	WrapperA::display(C& c)
{
	typename C::iterator	it;
	for (it = c.begin(); it != c.end(); it++)
		std::cout << *it << " ";
	std::cout << "\n";
}
template <>
void	WrapperA::insertion<std::vector<int> >(std::vector<int>& v, int x)
{
	std::cout << "inserting in vector\n";
	v.push_back(x);
}
template <>
void	WrapperA::insertion<std::deque<int> >(std::deque<int>& d, int x)
{
	std::cout << "inserting in deque\n";
	d.push_back(x);
}

class	WrapperB
{
	public:
		WrapperB() {};
		void	insertion(std::vector<int>& d, int x);
		void	insertion(std::deque<int>& d, int x);
		template <class C> void	display(C& c);
	private:
};
template <class C>
void	WrapperB::display(C& c)
{
	typename C::iterator	it;
	for (it = c.begin(); it != c.end(); it++)
		std::cout << *it << " ";
	std::cout << "\n";
}
void	WrapperB::insertion(std::vector<int>& v, int x)
{
	std::cout << "inserting in vector\n";
	v.push_back(x);
}
void	WrapperB::insertion(std::deque<int>& d, int x)
{
	std::cout << "inserting in deque\n";
	d.push_back(x);
}


int	main(void)
{
	int						arr[4] = {3, 5, 7, 9};
	std::vector<int>		v(arr + 0, arr + 4);
	std::deque<int>			d(arr + 0, arr + 4);
	WrapperA				wA;
	WrapperB				wB;

	wA.insertion(v, 10);
	wA.insertion(d, 11);
	wA.display(v);
	wA.display(d);

	wB.insertion(v, 20);
	wB.insertion(d, 21);
	wB.display(v);
	wB.display(d);
	
}
