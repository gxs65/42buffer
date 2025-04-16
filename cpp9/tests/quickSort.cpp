#include "quicksort.hpp"

void	logVector(std::vector<int>& arr)
{
	std::vector<int>::iterator	it;

	std::cout << "Int vector { ";
	for (it = arr.begin(); it != arr.end(); it++)
		std::cout << *it << " ";
	std::cout << "}\n";
}

void	swapInts(int* a, int *b)
{
	int	swapper;

	swapper = *a;
	*a = *b;
	*b = swapper;
}

int	quickSortPartitions(std::vector<int>& arr, int begin, int end, int pivot)
{
	int		asc = begin - 1;
	int		desc = end + 1;
	int		pivotValue = arr[pivot];

	while (1)
	{
		asc++;
		while (arr[asc] < pivotValue)
			asc++;
		desc--;
		while (arr[desc] > pivotValue)
			desc--;
		if (asc >= desc)
		{
			std::cout << "\tended partitioning with asc " << asc << " and desc " << desc << "\n";
			return (desc);
		}
		swapInts(&(arr[asc]), &(arr[desc]));
	}
}


void	quickSort(std::vector<int>& arr, int begin, int end)
{
	int	pivot;
	int	sep;

	std::cout << "recursion with indexes : begin " << begin << " and end " << end << "\n";
	if (begin >= end || begin < 0 || end < 0)
		return ;
	pivot = begin + (end - begin) / 2;
	std::cout << " -> pivot " << pivot <<" with value " << arr[pivot] << "\n";
	sep = quickSortPartitions(arr, begin, end, pivot);
	std::cout << "state of array after paritioning (sep at index " << sep << ") :\n";
	logVector(arr);
	quickSort(arr, begin, sep);
	quickSort(arr, sep + 1, end);
	std::cout << "at end of recursion with indexes begin " << begin << " and end " << end << ", state of array :\n";
	logVector(arr);
}
