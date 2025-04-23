#include <iostream>
#include <vector>
#include <list>
#include <deque>
#include <algorithm>

// Result of tests : functional for deque and vector,
// but not for list, since its iterator are NOT random-access iterators,
// so they do not provide operators '+'/'-' and ordered comparisons '<'/'>'

template <class C>
void	testIterator(C& v)
{
	typename C::iterator	iv;

	iv = v.begin();
	std::cout << "begin = " << *iv << "\n";
	iv = iv + 2;
	std::cout << "middle = " << *iv << "\n";
	iv = iv + 2;
	std::cout << "end = " << *iv << "\n";
	iv = iv + 1;
	std::cout << "iterator is past end : " << (iv >= v.end()) << "\n";
}

int	main(void)
{
	int						arr[4] = {3, 5, 7, 9};
	std::vector<int>		v(arr + 0, arr + 4);
	std::list<int>			l(arr + 0, arr + 4);
	std::deque<int>			d(arr + 0, arr + 4);
	//std::vector<int>::iterator	iv;
	//std::list<int>::iterator		il;
	//std::deque<int>::iterator		id;

	std::cout << "VECTOR :\n";
	testIterator(v);
	//std::cout << "\nLIST :\n";
	//testIterator(l);
	std::cout << "\nDEQUE :\n";
	testIterator(d);
}
