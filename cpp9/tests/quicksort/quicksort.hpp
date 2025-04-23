#ifndef QUICKSORT_H
# define QUICKSORT_H

# include <string>
# include <iostream>
# include <vector>

# define ARR_SIZE 10

void	quickSort(std::vector<int>& arr, int begin, int end);
void	logVector(std::vector<int>& arr);

#endif