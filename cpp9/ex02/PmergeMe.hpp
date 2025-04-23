#ifndef PMERGEME_H
# define PMERGEME_H

# include <string>
# include <sstream>
# include <iostream>
# include <fstream>
# include <map>
# include <vector>
# include <algorithm>
# include <stdlib.h>
# include <iomanip>

# define MAX_UINT 4294967295

class	PmergeMe
{
	public:
		PmergeMe(void);
		PmergeMe(const PmergeMe& orig);
		~PmergeMe(void);
		PmergeMe&			operator= (const PmergeMe& orig);
		void				sort(std::vector<int>& v, unsigned int top, int recurrLvl, unsigned int elementSize);
		
	private:
		std::vector<unsigned int>	jacobsthalNumbers;
		void						generateJacobsthalNumbers(int n);
		void						swapElements(std::vector<int>& v, unsigned int indFirstNumber,
										unsigned int elementSize);
		void						swapNumbers(std::vector<int>& v, unsigned int indA, unsigned int indB);
		void						insertElement(std::vector<int>& dest, std::vector<int>& src,
										unsigned int indInDest, unsigned int indInSrc, unsigned int elementSize);
		std::vector<int>::iterator	binaryInsertionIndex(std::vector<int>& v, unsigned int elementSize,
										std::vector<int>::iterator down, std::vector<int>::iterator top, int n);
};

// Utils functions
int		logError(std::string msg);
bool	isDigit(char c);
bool	isSorted(std::vector<int> v);
bool	isValidNumber(std::string valueStr);
void	strToIntArray(std::vector<int>& v, std::string s);
void	argsToIntArray(std::vector<int>& v, char **av);
template <class T> void	logVector(std::vector<T>& arr);


#endif
