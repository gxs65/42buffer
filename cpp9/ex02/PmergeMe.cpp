#include "PmergeMe.hpp"

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

PmergeMe::PmergeMe(void)
{
	std::cout << "Default constructor for PmergeMe\n";
	this->generateJacobsthalNumbers(12);
	std::cout << "Jacobsthal numbers used for Ford-Johnson sort :\n";
	logVector(this->jacobsthalNumbers);
}

PmergeMe::PmergeMe(const PmergeMe& orig)
{
	(void)orig;
	std::cout << "Copy constructor for PmergeMe\n";
}

PmergeMe::~PmergeMe(void)
{
	std::cout << "Destructor for PmergeMe\n";
}

PmergeMe&	PmergeMe::operator= (const PmergeMe& orig)
{
	std::cout << "Assignment operator overload for PmergeMe\n";
	if (this != &orig)
		;
	return (*this);
}

///////////////
// ACCESSORS //
///////////////

///////////////////////////
// SEQUENCE MANIPULATION //
///////////////////////////

void	PmergeMe::insertElement(std::vector<int>& dest, std::vector<int>& src,
	unsigned int indInDest, unsigned int indInSrc, unsigned int elementSize)
{
	unsigned int				indInElement;
	int							number;
	std::vector<int>::iterator	destBegin = dest.begin();
	(void)destBegin;

	std::cout << "\t\t\tinserting element size " << elementSize << " at index " << indInSrc
		<< " of src, into index " << indInDest << " of dest, current state of dest : ";
	logVector(dest);
	if (indInDest == MAX_UINT)
	{
		for (indInElement = 0; indInElement < elementSize; indInElement++)
		{
			number = src[indInSrc + indInElement];
			dest.insert(dest.end(), number);
		}
	}
	else
	{
		for (indInElement = 0; indInElement < elementSize; indInElement++)
		{
			number = src[indInSrc + indInElement];
			//std::cout << "\t\t\t\tat index in element " << indInElement << ", inserting number "
			//	<< number << " from src into position " << indInDest + indInElement << " of dest\n"; 
			dest.insert(dest.begin() + indInDest + indInElement, number);
		}
	}
}

void	PmergeMe::swapNumbers(std::vector<int>& v, unsigned int indA, unsigned int indB)
{
	int	swapper;

	swapper = v[indA];
	v[indA] = v[indB];
	v[indB] = swapper;
}

void	PmergeMe::swapElements(std::vector<int>& v, unsigned int indFirstNumber, unsigned int elementSize)
{
	unsigned int	indInElement;
 
	for (indInElement = 0; indInElement < elementSize; indInElement++)
		this->swapNumbers(v, indFirstNumber + indInElement, indFirstNumber + elementSize + indInElement);
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
	std::cout << "\t\t\tbinaryInsertion of " << n << " with *down = " << *down << ", *top = " << *top
		<< ", elemsize = " << elementSize << ", *mid = " << *mid << "\n"; 
	// case '=' is excluded
	if (n > *mid)
		return (this->binaryInsertionIndex(v, elementSize, mid + 1, top, n));
	else
		return (this->binaryInsertionIndex(v, elementSize, down, mid, n));
}
*/

// down included, top excluded
std::vector<int>::iterator	PmergeMe::binaryInsertionIndex(std::vector<int>& v, unsigned int elementSize,
	std::vector<int>::iterator down, std::vector<int>::iterator top, int n)
{
	std::vector<int>::iterator	mid;

	if (down == top)
		return (down);
	mid = down + elementSize * ((top - down) / (elementSize * 2));
	std::cout << "\t\t\tbinaryInsertion of " << n << " (ending element of size " << elementSize
		<< ") ; with *down = " << *(down + elementSize - 1);
	if (top == v.end())
		std::cout << ", *top = end";
	else
		std::cout << ", *top = " << *(top + elementSize - 1);
	std::cout << ", *mid = " << *(mid + elementSize - 1) << "\n";
	// case '=' is excluded
	if (n > *(mid + elementSize - 1))
		return (this->binaryInsertionIndex(v, elementSize, mid + elementSize, top, n));
	else
		return (this->binaryInsertionIndex(v, elementSize, down, mid, n));
}


////////////////////
// MAIN INTERFACE //
////////////////////

void	testSort(std::vector<int>& v)
{
	int 				arr[8] = {0, 3, 2, 4, 1, 5, 6, 7};
	std::vector<int>	vc(&(arr[0]), &(arr[8]));

	v = vc;
}

void	PmergeMe::sort(std::vector<int>& v, unsigned int top, int recurrLvl, unsigned int elementSize)
{
	unsigned int				pairSize = elementSize * 2;
	unsigned int				ind;
	unsigned int				indSmall;
	unsigned int				indPend;
	int							currentTwoPower;
	int							currentInsertionSeqSize;
	std::vector<int>::iterator	insertionPosition;
	unsigned int				currentJacobsthalNumber;
	unsigned int				previousJacobsthalNumber;

	std::cout << "Lvl " << recurrLvl << ", elemsize " << elementSize << " - SORT CALLED ; state of sequence :\n";
	logVector(v);
	if (elementSize * 2 > v.size())
		return ;
	std::cout << "Lvl " << recurrLvl << ", elemsize " << elementSize << " - A) creating and ordering pairs "
		<< "; state of sequence :\n";
	for (ind = 0; ind + pairSize - 1 <= top; ind += pairSize)
	{
		if (v[ind + elementSize - 1] > v[ind + 2 * elementSize - 1])
			this->swapElements(v, ind, elementSize);
	}
	logVector(v);

	// sorting the pairs by their highest element, through recursivity
	this->sort(v, top, recurrLvl + 1, elementSize * 2);
	//testSort(v);

	std::cout << "\nLvl " << recurrLvl << ", elemsize " << elementSize << " - B) creating main chain "
		<< "; state of sequence after recursion :\n";
	logVector(v);
	std::vector<int>	mainChain;
	std::vector<int>	pending;
	std::vector<int>	remainder;
	// at this point, we know that there is at least 1 pair at this recursion level
	this->insertElement(mainChain, v, MAX_UINT, 0, elementSize);
	this->insertElement(mainChain, v, MAX_UINT, 0 + elementSize, elementSize);
	// but there could be only 1 pair, so this loop could be executed 0 times
	for (ind = 0 + pairSize; ind + pairSize - 1 <= top; ind += pairSize)
	{
		this->insertElement(pending, v, MAX_UINT, ind, elementSize);
		this->insertElement(mainChain, v, MAX_UINT, ind + elementSize, elementSize);
	}
	if (ind + elementSize - 1 <= top)
	{
		this->insertElement(pending, v, MAX_UINT, ind, elementSize);
		ind += elementSize;
	}
	while (ind < v.size())
	{
		remainder.insert(remainder.end(), v[ind]);
		ind++;
	}
	std::cout << "Before inserting small elements :\n\tmain chain is : ";
	logVector(mainChain);
	std::cout << "\tpending is : ";
	logVector(pending);
	std::cout << "\tremainder is : ";
	logVector(remainder);

	std::cout << "\nLvl " << recurrLvl << ", elemsize " << elementSize << " - C) inserting pending elements\n";
	currentTwoPower = 1;
	currentInsertionSeqSize = 1;
	currentJacobsthalNumber = 1;
	previousJacobsthalNumber = 1;
	while (currentJacobsthalNumber < pending.size() + 2) // -2 lhs passed to rhs
	{
		currentTwoPower++;
		currentInsertionSeqSize = (currentInsertionSeqSize + 1) * 2 - 1;
		currentJacobsthalNumber = this->jacobsthalNumbers[currentTwoPower];
		previousJacobsthalNumber = this->jacobsthalNumbers[currentTwoPower - 1];
		std::cout << "\tat 2^" << currentTwoPower << ", insertion in seq of size " << currentInsertionSeqSize
			<< ", inserting small element from index " << currentJacobsthalNumber
			<< " incl to " << previousJacobsthalNumber << " excl\n";
		for (indSmall = currentJacobsthalNumber; indSmall > previousJacobsthalNumber; indSmall--)
		{
			indPend = (indSmall - 2) * elementSize; // -1 because indexes defined by Jacobsthal numbers begin at 1
													// -1 again because <pending> begins with small element of index 2 instead of 1
			if (indPend < pending.size())
			{
				std::cout << "\t\tinserting small element index " << indSmall << " -> index in pend " << indPend
					<< " -> value " << pending[indPend + elementSize - 1] << "\n";
				if (currentInsertionSeqSize * elementSize >= mainChain.size())
				{
					insertionPosition = this->binaryInsertionIndex(mainChain, elementSize,
						mainChain.begin(), mainChain.end(),
						pending[indPend + elementSize - 1]);
				}
				else
				{
					insertionPosition = this->binaryInsertionIndex(mainChain, elementSize,
						mainChain.begin(), mainChain.begin() + currentInsertionSeqSize * elementSize,
						pending[indPend + elementSize - 1]);
				}
				this->insertElement(mainChain, pending,
					insertionPosition - mainChain.begin(), indPend, elementSize);
				std::cout << "\t\t\tmain chain after insertion : ";
				logVector(mainChain);
			}
		}
	}
	v.clear();
	v = mainChain;
	v.insert(v.end(), remainder.begin(), remainder.end());
}

////////////////////
// INITIALIZATION //
////////////////////

// Generates the sequence of Jacobsthal numbers up to index <n> included,
// inside the private attribute <jacobsthalNumber>
void	PmergeMe::generateJacobsthalNumbers(int n)
{
	int	ind;
	int	j;

	this->jacobsthalNumbers.push_back(1);
	this->jacobsthalNumbers.push_back(1);
	ind = 2;
	while (ind <= n)
	{
		j = this->jacobsthalNumbers[ind - 1] + 2 * this->jacobsthalNumbers[ind - 2];
		this->jacobsthalNumbers.push_back(j);
		ind++;
	}
}

/////////////////////
// UTILS FUNCTIONS //
/////////////////////

// Logs an error, then returns 1
int		logError(std::string msg)
{
	std::cout << "Error : " << msg << "\n";
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

// Checks if the number sequence is sorted
bool	isSorted(std::vector<int> v)
{
	std::vector<int>::iterator	it;

	for (it = v.begin(); it != v.end(); it++)
	{
		if (it != v.begin() && *it < *(it - 1))
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

// Displays the current state of the number sequence
template <class T>
void	logVector(std::vector<T>& arr)
{
	typename std::vector<T>::iterator	it;

	std::cout << "Int vector { ";
	for (it = arr.begin(); it != arr.end(); it++)
		std::cout << *it << " ";
	std::cout << "}\n";
}
