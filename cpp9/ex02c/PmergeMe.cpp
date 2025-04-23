#include "PmergeMe.hpp"

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

PmergeMe::PmergeMe(unsigned int seqSize)
{
	int		ind;
	int		jacobsthalSize;

	std::clog << "Default constructor for PmergeMe\n";
	jacobsthalSize = this->generateJacobsthalNumbers(seqSize);
	std::clog << "Jacobsthal numbers used for Ford-Johnson sort : { ";
	for (ind = 0; ind < jacobsthalSize; ind++)
		std::clog << this->jacobsthalNumbers[ind] << " ";
	std::clog << "}\n";
	this->nbComps = 0;
}

PmergeMe::PmergeMe(const PmergeMe& orig)
{
	(void)orig;
	std::clog << "Copy constructor for PmergeMe\n";
}

PmergeMe::~PmergeMe(void)
{
	std::clog << "Destructor for PmergeMe\n";
	delete[] this->jacobsthalNumbers;
}

PmergeMe&	PmergeMe::operator= (const PmergeMe& orig)
{
	std::clog << "Assignment operator overload for PmergeMe\n";
	if (this != &orig)
		;
	return (*this);
}

///////////////
// ACCESSORS //
///////////////

int	PmergeMe::getNbComps(void) const
{
	return (this->nbComps);
}

///////////////////////////
// SEQUENCE MANIPULATION //
///////////////////////////

void	PmergeMe::insertElement(std::vector<int>& dest, std::vector<int>& src,
	std::vector<int>::iterator posInDest, std::vector<int>::iterator posInSrc,
	unsigned int elementSize)
{
	unsigned int				indInElement;
	int							number;

	//std::clog << "\t\t\tinserting element ";
	//logVectorAt(src, posInSrc, elementSize, 0);
	//std::clog << "into dest ";
	//logVectorElements(dest, 0, elementSize);
	//if (posInDest == dest.end())
	//	std::clog << "at end\n";
	//else
	//	std::clog << "at position currently held by nb " << *posInDest << "\n";
	(void)dest;
	(void)src;
	for (indInElement = 0; indInElement < elementSize; indInElement++)
	{
		number = *(posInSrc + indInElement);
		//std::clog << "\t\t\t\t" << indInElement << "th element is " << number << " inserted in ";
		//logVector(dest, 1);
		posInDest = dest.insert(posInDest, number);
		posInDest++;
	}
}

void	PmergeMe::swapNumbers(std::vector<int>::iterator posA, std::vector<int>::iterator posB)
{
	int	swapper;

	swapper = *posA;
	*posA = *posB;
	*posB = swapper;
}

void	PmergeMe::swapElements(std::vector<int>::iterator posFirstNumber, unsigned int elementSize)
{
	unsigned int	indInElement;
 
	for (indInElement = 0; indInElement < elementSize; indInElement++)
		this->swapNumbers(posFirstNumber + indInElement, posFirstNumber + elementSize + indInElement);
}

/*
// down included, top excluded
std::vector<int>::iterator	PmergeMe::binaryInsertionIndex(std::vector<int> v, unsigned int elementSize,
	std::vector<int>::iterator down, std::vector<int>::iterator top, int n)
{
	std::vector<int>::iterator	mid;

	if (top == down)
		return (top);
	mid = down + ((top - down) / 2);
	std::clog << "\t\t\tbinaryInsertion of " << n << " with *down = " << *down << ", *top = " << *top
		<< ", elemsize = " << elementSize << ", *mid = " << *mid << "\n"; 
	// case '=' is excluded
	if (n > *mid)
		return (this->binaryInsertionIndex(v, elementSize, mid + 1, top, n));
	else
		return (this->binaryInsertionIndex(v, elementSize, down, mid, n));
}
*/

//////////////////////
// BINARY INSERTION //
//////////////////////

bool	PmergeMe::isStrictlyGreater(int a, int b)
{
	this->nbComps++;
	return (a > b);
}

// down included, top excluded
std::vector<int>::iterator	PmergeMe::binaryInsertionIndex(std::vector<int>& v, unsigned int elementSize,
	std::vector<int>::iterator down, std::vector<int>::iterator top, int n)
{
	std::vector<int>::iterator	mid;

	if (down == top)
		return (down);
	mid = down + elementSize * ((top - down) / (elementSize * 2));
	std::clog << "\t\t\tbinaryInsertion of " << n << " (ending element of size " << elementSize
		<< ") ; with *down = " << *(down + elementSize - 1);
	if (top == v.end())
		std::clog << ", *top = end";
	else
		std::clog << ", *top = " << *(top + elementSize - 1);
	std::clog << ", *mid = " << *(mid + elementSize - 1) << "\n";
	// case '=' is excluded
	if (this->isStrictlyGreater(n, *(mid + elementSize - 1)))
		return (this->binaryInsertionIndex(v, elementSize, mid + elementSize, top, n));
	else
		return (this->binaryInsertionIndex(v, elementSize, down, mid, n));
}


////////////////////
// MAIN INTERFACE //
////////////////////

void	PmergeMe::stepA_Pairing(std::vector<int>& v, int recurrLvl, unsigned int elementSize)
{
	unsigned int				pairSize = elementSize * 2;
	std::vector<int>::iterator	pos;

	std::clog << "Lvl " << recurrLvl << ", elemsize " << elementSize << " - A) creating and ordering pairs\n";
	for (pos = v.begin(); pos + pairSize - 1 < v.end(); pos += pairSize)
	{
		if (this->isStrictlyGreater(*(pos + elementSize - 1), *(pos + 2 * elementSize - 1)))
			this->swapElements(pos, elementSize);
	}
	std::clog << "state of sequence after pair ordering : ";
	logVectorElements(v, 1, elementSize * 2);
}

void	PmergeMe::stepB_Dividing(std::vector<int>& v, int recurrLvl, unsigned int elementSize,
	std::vector<int>& mainChain, std::vector<int>& pending, std::vector<int>& remainder)
{
	unsigned int				pairSize = elementSize * 2;
	std::vector<int>::iterator	pos;

	std::clog << "Lvl " << recurrLvl << ", elemsize " << elementSize << " - B) creating main chain\n";
	// We know that there is at least 1 pair at this recursion level
	// so this loop must be executed at least 1 time
	// First small element is inserted in both mainChain and pending, for convenience
	std::clog << "\tbig elements -> main chain ; small elements -> pending\n";
	for (pos = v.begin(); pos + pairSize - 1 < v.end(); pos += pairSize)
	{
		if (pos == v.begin())
			this->insertElement(mainChain, v, mainChain.end(), pos, elementSize);
		this->insertElement(pending, v, pending.end(), pos, elementSize);
		this->insertElement(mainChain, v, mainChain.end(), pos + elementSize, elementSize);
	}
	if (pos + elementSize - 1 < v.end()) // element that could not form a pair, because nbElements is uneven
	{
		std::clog << "\tlast uneven element -> pending\n";
		this->insertElement(pending, v, pending.end(), pos, elementSize);
		pos += elementSize;
	}
	std::clog << "\tlast numbers insufficient to form an element -> remainder\n";
	while (pos != v.end()) // numbers that could not form an element
	{
		remainder.insert(remainder.end(), *pos);
		pos++;
	}
	std::clog << "Before inserting small elements :\n\tmain chain is : ";
	logVectorElements(mainChain, 1, elementSize);
	std::clog << "\tpending is : ";
	logVectorElements(pending, 1, elementSize);
	std::clog << "\tremainder is : ";
	logVector(remainder, 1);
}

void	PmergeMe::stepC_Inserting2(unsigned int elementSize,
	std::vector<int>& mainChain, std::vector<int>& pending,
	std::vector<int>::iterator posExtraction, std::vector<int>::iterator posLastExtracted,
	int currentInsertionSeqSize)
{
	std::vector<int>::iterator	posInsertion; // in <mainChain>
	std::vector<int>::iterator	posInsertionMax; // in <mainChain>

	while (posExtraction != posLastExtracted)
	{
		std::clog << "\t\tinserting small element ";
		logVectorAt(pending, posExtraction, elementSize, 1);
		if (currentInsertionSeqSize * elementSize >= mainChain.size())
			posInsertionMax = mainChain.end();
		else
			posInsertionMax = mainChain.begin() + currentInsertionSeqSize * elementSize;
		posInsertion = this->binaryInsertionIndex(mainChain, elementSize,
				mainChain.begin(), posInsertionMax,
				*(posExtraction + elementSize - 1));
		this->insertElement(mainChain, pending,
			posInsertion, posExtraction, elementSize);
		std::clog << "\t\t\tmain chain after insertion : ";
		logVectorElements(mainChain, 1, elementSize);
		posExtraction -= elementSize;
	}
}

// while there are numbers to insert in <pending>
// (<previousJNumber> is the number of already inserted elements)
void	PmergeMe::stepC_Inserting(int recurrLvl, unsigned int elementSize,
	std::vector<int>& mainChain, std::vector<int>& pending)
{
	int							currentTwoPower = 2;
	int							currentInsertionSeqSize = 3;
	unsigned int				currentJacobsthalNumber = 3;
	unsigned int				previousJacobsthalNumber = 1;
	std::vector<int>::iterator	posLastExtracted; // in <pending>
	std::vector<int>::iterator	posExtraction; // in <pending>

	std::clog << "\nLvl " << recurrLvl << ", elemsize " << elementSize << " - C) inserting pending elements\n";
	posLastExtracted = pending.begin();
	while (pending.size() > previousJacobsthalNumber * elementSize)
	{
		std::clog << "\tat 2^" << currentTwoPower << ", insertion in seq of size " << currentInsertionSeqSize
			<< " (-> max " << currentTwoPower << " comparisons),\n"
			<< "\tinserting small elements from index j(" << currentTwoPower << ") = " << currentJacobsthalNumber
			<< " incl to j(" << currentTwoPower - 1 << ") = " << previousJacobsthalNumber << " excl\n"
			<< "\t<=> converted as indexes of <pending>, from " << currentJacobsthalNumber - 1
			<< " incl to " << previousJacobsthalNumber - 1 << " excl\n";
		if (pending.size() >= currentJacobsthalNumber * elementSize)
			posExtraction = posLastExtracted + elementSize * (currentJacobsthalNumber - previousJacobsthalNumber);
		else
			posExtraction = pending.end() - elementSize;
		this->stepC_Inserting2(elementSize, mainChain, pending, posExtraction, posLastExtracted, currentInsertionSeqSize);
		posLastExtracted = posLastExtracted + elementSize * (currentJacobsthalNumber - previousJacobsthalNumber);
		currentTwoPower++;
		currentInsertionSeqSize = (currentInsertionSeqSize + 1) * 2 - 1;
		currentJacobsthalNumber = this->jacobsthalNumbers[currentTwoPower];
		previousJacobsthalNumber = this->jacobsthalNumbers[currentTwoPower - 1];
	}
}

void	PmergeMe::sort(std::vector<int>& v, int recurrLvl, unsigned int elementSize)
{
	std::vector<int>::iterator	it;
	std::vector<int>::iterator	itPending;
	std::vector<int>::iterator	itLimitInsertion;
	std::vector<int>			mainChain;
	std::vector<int>			pending;
	std::vector<int>			remainder;

	std::clog << "\nLvl " << recurrLvl << ", elemsize " << elementSize << " - SORT CALLED ; state of sequence : ";
	logVector(v, 1);
	if (elementSize * 2 > v.size())
		return ;
	this->stepA_Pairing(v, recurrLvl, elementSize);

	this->sort(v, recurrLvl + 1, elementSize * 2);
	std::clog << "\nLvl " << recurrLvl << ", elemsize " << elementSize << " - state of sequence after recursion : ";
	logVectorElements(v, 1, elementSize);
	
	this->stepB_Dividing(v, recurrLvl, elementSize, mainChain, pending, remainder);
	this->stepC_Inserting(recurrLvl, elementSize, mainChain, pending);
	
	v.clear();
	v.insert(v.end(), mainChain.begin(), mainChain.end());
	v.insert(v.end(), remainder.begin(), remainder.end());
	std::clog << "\nLvl " << recurrLvl << ", elemsize " << elementSize << " - state of sequence after all steps : ";
	logVectorElements(v, 1, elementSize);
}

////////////////////
// INITIALIZATION //
////////////////////

// Generates the sequence of Jacobsthal numbers
// inside the private attribute <jacobsthalNumber>,
// 		up to the first Jacobsthal number that is higher
// 		than the size of the sequence to sort
// 		(to be certain that enough Jacobsthal number are generated)
int	PmergeMe::generateJacobsthalNumbers(unsigned int seqSize)
{
	int	ind;
	int	j;

	this->jacobsthalNumbers = new unsigned int [50];
	this->jacobsthalNumbers[0] = 1;
	this->jacobsthalNumbers[1] = 1;
	ind = 2;
	while (this->jacobsthalNumbers[ind - 1] <= seqSize)
	{
		j = this->jacobsthalNumbers[ind - 1] + 2 * this->jacobsthalNumbers[ind - 2];
		this->jacobsthalNumbers[ind] = j;
		ind++;
	}
	return (ind);
}

/////////////////////
// UTILS FUNCTIONS //
/////////////////////

// Logs an error, then returns 1
int		logError(std::string msg)
{
	std::clog << "Error : " << msg << "\n";
	return (1);
}

// Checks if a character in the digits range of the ASCII table
bool	isDigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

// Checks if the number sequence is sorted (values are consecutive and growing)
bool	isSorted(std::vector<int> v)
{
	std::vector<int>::iterator	it;

	for (it = v.begin(); it != v.end(); it++)
	{
		if (it != v.begin() && *it - *(it - 1) != 1)
			return (0);
	}
	return (1);
}

// Checks that a literal representing an int (a number in the sequence) is valid,
// ie contains only digits and potentially a '+'/'-' leading sign
bool	isValidNumber(std::string valueStr)
{
	unsigned int	ind = 0;
	int				nb_digits_passed = 0;

	if (valueStr[0] == '+' || valueStr[0] == '-')
		ind++;
	while (ind < valueStr.length())
	{
		if (isDigit(valueStr[ind]))
			nb_digits_passed++;
		else
			return (0);
		ind++;
	}
	if (nb_digits_passed == 0)
		return (0);
	else
		return (1);
}

// Converts a string containing int literals separated by spaces
// into a sequence of ints in a container
void	strToIntArray(std::vector<int>& v, std::string s)
{
	std::stringstream	parserStream;
	std::string			token;

	parserStream << s;
	while (std::getline(parserStream, token, ' '))
	{
		if (isValidNumber(token))
			v.push_back(static_cast<int>(strtol(token.c_str(), NULL, 10)));
	}
}

// Converts an arrays of c-style strings each containing one int literal
// into a sequence of ints in a container
void	argsToIntArray(std::vector<int>& v, char **av)
{
	int	ind = 0;

	while (av[ind])
	{
		if (isValidNumber(std::string(av[ind])))
			v.push_back(static_cast<int>(strtol(av[ind], NULL, 10)));
		ind++;
	}
}

void	logVector(std::vector<int>& arr, int endingLinebreak)
{
	std::vector<int>::iterator	it;

	std::clog << "{ ";
	for (it = arr.begin(); it != arr.end(); it++)
		std::clog << *it << " ";
	if (endingLinebreak)
		std::clog << "}\n";
	else
		std::clog << "} ";
}

void	logVectorElements(std::vector<int>& arr, int endingLinebreak, unsigned int elementSize)
{
	std::vector<int>::iterator	it;
	unsigned int				ind = 0;

	std::clog << "{ ";
	for (it = arr.begin(); it != arr.end(); it++)
	{
		if (ind % elementSize == elementSize - 1)
			std::clog << "\033[32m" << *it << "\033[0m ";
		else
			std::clog << *it << " ";
		ind++;
	}
	if (endingLinebreak)
		std::clog << "}\n";
	else
		std::clog << "} ";
}

void	logVectorAt(std::vector<int>& arr,
	std::vector<int>::iterator start, unsigned int nbNumbers, int endingLinebreak)
{
	unsigned int	ind;

	(void)arr;
	std::clog << "{[ ";
	for (ind = 0; ind < nbNumbers; ind++)
	{
		if (ind == nbNumbers - 1)
			std::clog << "\033[32m" << *(start + ind) << "\033[0m ";
		else
			std::clog << *(start + ind) << " ";
	}
	if (endingLinebreak)
		std::clog << "]}\n";
	else
		std::clog << "]} ";
}
