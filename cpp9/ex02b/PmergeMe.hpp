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
		PmergeMe(unsigned int seqSize);
		PmergeMe(const PmergeMe& orig);
		~PmergeMe(void);
		PmergeMe&			operator= (const PmergeMe& orig);
		int					getNbComps(void) const;
		void				sort(std::vector<int>& v, int recurrLvl, unsigned int elementSize);
		
	private:
		unsigned int				*jacobsthalNumbers;
		int							nbComps;
		int							generateJacobsthalNumbers(unsigned int seqSize);
		bool						isStrictlyGreater(int a, int b);
		// Manipulation of elements inside the sequence
		void						swapElements(std::vector<int>::iterator posFirstNumber, unsigned int elementSize);
		void						swapNumbers(std::vector<int>::iterator posA, std::vector<int>::iterator posB);
		void						insertElement(std::vector<int>& dest, std::vector<int>& src,
										std::vector<int>::iterator posInDest, std::vector<int>::iterator posInSrc,
										unsigned int elementSize);
		std::vector<int>::iterator	binaryInsertionIndex(std::vector<int>& v, unsigned int elementSize,
										std::vector<int>::iterator down, std::vector<int>::iterator top, int n);
		// The 3 steps in function <PmergeMe::sort>
		void						stepA_Pairing(std::vector<int>& v, int recurrLvl, unsigned int elementSize);
		void						stepB_Dividing(std::vector<int>& v, int recurrLvl, unsigned int elementSize,
										std::vector<int>& mainChain, std::vector<int>& pending,
										std::vector<int>& remainder);
		void						stepC_Inserting(int recurrLvl, unsigned int elementSize,
										std::vector<int>& mainChain, std::vector<int>& pending);
		void						stepC_Inserting2(unsigned int elementSize,
										std::vector<int>& mainChain, std::vector<int>& pending,
										std::vector<int>::iterator posExtraction, std::vector<int>::iterator posLastExtracted,
										int currentInsertionSeqSize);

};

// Utils functions
int		logError(std::string msg);
bool	isDigit(char c);
bool	isSorted(std::vector<int> v);
bool	isValidNumber(std::string valueStr);
void	strToIntArray(std::vector<int>& v, std::string s);
void	argsToIntArray(std::vector<int>& v, char **av);

void	logVector(std::vector<int>& arr, int endingLinebreak);
void	logVectorElements(std::vector<int>& arr, int endingLinebreak, unsigned int elementSize);
void	logVectorAt(std::vector<int>& arr, std::vector<int>::iterator start, unsigned int nbNumbers, int endingLinebreak);

#endif
