#include "BitcoinExchange.hpp"

// gerer le cas database vide

/////////////////////////////
// CANONICAL ORTHODOX FORM //
/////////////////////////////

BitcoinExchange::BitcoinExchange(void)
{
	std::cout << "Default constructor for BitcoinExchange\n";
}

BitcoinExchange::BitcoinExchange(std::string recordFileName)
{
	std::cout << "Parameter constructor for BitcoinExchange\n";
	this->parseDatabase(recordFileName);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& orig)
{
	std::cout << "Copy constructor for BitcoinExchange\n";
	this->bitcoinRatesRecord = orig.getRecord();
}

BitcoinExchange::~BitcoinExchange(void)
{
	std::cout << "Destructor for BitcoinExchange\n";
}

BitcoinExchange&	BitcoinExchange::operator= (const BitcoinExchange& orig)
{
	std::cout << "Assignment operator overload for BitcoinExchange\n";
	if (this != &orig)
		this->bitcoinRatesRecord = orig.getRecord();
	return (*this);
}

///////////////
// ACCESSORS //
///////////////

const std::map<std::string, double>&	BitcoinExchange::getRecord(void) const
{
	return (this->bitcoinRatesRecord);
}

////////////////////
// MAIN INTERFACE //
////////////////////

// Reads the file containing bitcoin rates line by line,
// and records the "date->bitcoinRate" information in the <record> map
// /!\ <record> map has key-value pairs of type <std::string, double> :
// 	   the bitcoin rate is converted to double, but the date is kept as string
// 			because thanks to date format "YYYY-MM-DD", the alphabetic order of date strings,
// 			corresponds to their chronological order,
// 		so that entries are, as required, inserted in chronological order in <record>
// 		and searching the map with <record.upper_bound> will yield the correct result 
int	BitcoinExchange::parseDatabase(std::string recordFileName)
{
	std::ifstream					recordFile;
	std::string						line;
	std::pair<std::string, double>	lineData;

	recordFile.open(recordFileName.c_str());
	std::getline(recordFile, line);
	while (std::getline(recordFile, line))
	{
		if (!(this->parseFileLine(lineData, line, 0)))
			this->bitcoinRatesRecord.insert(lineData);
	}
	recordFile.close();
	std::cout << "Database file '" << recordFileName << "' parsed\n";
	//logMap(this->bitcoinRatesRecord);
	return (0);
}

// (Most important function for this exercise)
// Searches the map for the data line whose date is (as required by the subject)
// 		\ smaller than the input date <dateStr>
// 		\ closest to input date <dateStr>
// We use the map method <upper_bound(key)>, which returns an iterator
// to the first element of the map whose key is STRICTLY AFTER the given key
// 		(so if there exists an element with with a key exactly equal to the given key,
// 		 <upper_bound> will return the element AFTER that element)
// and then we decrement the returned iterator to get to the correct element
// 		(except if the returned element is the first element of map)
// Some examples :
// 		\ the given date is smaller than all dates in map :
// 			<upper_bound> returns iterator to <begin>, it is not decremented
// 		\ the given date is present in map at p :
// 			<upper_bound> returns iterator to p+1, it is decremented so it points to p
// 		\ the given date is between date of element at p and date of element at p+1 :
// 			<upper_bound> returns iterator to p+1, it is decremented so it points to p
// 		\ the given date is greater than all dates in map :
// 			<upper_bound> returns an iterator to <end>, it is decremented so it points to last element
int	BitcoinExchange::fetchBitcoinRateAt(std::string dateStr,
	std::pair<std::string, double>& closestRecordData)
{
	std::map<std::string, double>::iterator	pos;

	if (this->bitcoinRatesRecord.empty())
		return (1);
	pos = this->bitcoinRatesRecord.upper_bound(dateStr);
	if (pos != this->bitcoinRatesRecord.begin()) // after last date in record
		pos--;
	closestRecordData = *pos;
	return (0);
}

// Displays how much the number of bitcoins given in <amountInfo.second>
// are worth according to the bitcoin rate given by <rateInfo.second>
void	BitcoinExchange::displayBitcoinTotalValue(std::pair<std::string, double>& amountInfo,
	std::pair<std::string, double>& rateInfo)
{
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "For date " << amountInfo.first
		<< ", closest record date is " << rateInfo.first << " :\n";
	std::cout << "\t\t1.00 B = " << rateInfo.second << " ---> " << amountInfo.second
		<< " B = " << rateInfo.second * amountInfo.second << "\n";
}

// Reads the file containing bitcoin amounts line by line,
// fetches the closest line in the bitcoin rate record
// and displays the translation
int	BitcoinExchange::translateBitcoinAmounts(std::string inputFileName)
{
	std::ifstream					inputFile;
	std::string						line;
	std::pair<std::string, double>	lineData;
	std::pair<std::string, double>	closestRecordData;

	inputFile.open(inputFileName.c_str());
	std::getline(inputFile, line);
	while (std::getline(inputFile, line))
	{
		if (!(this->parseFileLine(lineData, line, 1)))
		{
			if (this->fetchBitcoinRateAt(lineData.first, closestRecordData))
				std::cout << "No bitcoin rate recorded, translation impossible\n";
			else
				this->displayBitcoinTotalValue(lineData, closestRecordData);
		}
	}
	inputFile.close();
	return (0);
}

////////////////////////
// PARSING AND CHECKS //
////////////////////////

// Checks if the date string :
// 		\ is exactly 3 date numbers separated by '-'
// 		\ where each date number is a 4 (year) or 2 (month/day) digits number
// 		\ where the year is not 0, and the month/day are in the expected range
bool	BitcoinExchange::isValidDate(std::string dateStr)
{
	std::string				buffer;
	std::stringstream		parserStream;
	unsigned int			expectedLength[3] = {4, 2, 2};
	long					dateNumbers[3];
	int						ind = 0;

	parserStream << dateStr;
	while (std::getline(parserStream, buffer, '-'))
	{
		if (ind >= 3)
			return (0); // too much date numbers
		if (buffer.length() != expectedLength[ind] || !isValidDateNumber(buffer))
			return (0); // date number with the wrong number of characters / non-digit characters
		dateNumbers[ind] = strtol(buffer.c_str(), NULL, 10);
		ind++;
	}
	if (ind != 3)
		return (0); // not enough date numbers
	if (dateNumbers[0] == 0 || dateNumbers[1] > 12 || dateNumbers[2] > 31)
		return (0); // year is 0, or month is not in range 1-12, or day is not in range 1-31
	if (dateNumbers[1] == 2 && ((isBissextileYear(dateNumbers[0]) && dateNumbers[2] > 29)
							|| (!isBissextileYear(dateNumbers[0]) && dateNumbers[2] > 28)))
		return (0); // day of February is not in range 1-28 or 1-29
	if ((dateNumbers[1] == 4 || dateNumbers[1] == 6
		|| dateNumbers[1] == 9 || dateNumbers[1] == 11) && dateNumbers[2] > 30)
		return (0); // day of 30-days month is not in range 1-30
	return (1);
}

// Checks that a literal representing a double (bitcoin rate) is valid
// Constraints on string : only digit characters ('0'-'9'),
// except for one unique decimal point ('.')
// 		-> in particular, negative numbers ('-') and special values ('inf'/'nan')
// 		   are considered invalid entries
bool	BitcoinExchange::isValidValue(std::string valueStr)
{
	unsigned int	ind = 0;
	bool			decimal_point_passed = 0;
	int				nb_digits_passed = 0;

	while (ind < valueStr.length())
	{
		if (valueStr[ind] == '.' && !decimal_point_passed)
			decimal_point_passed = 1;
		else if (isDigit(valueStr[ind]))
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

// Reads one line of bitcoin information containing a date and a number
// and puts its content into the first and second elements of the pair <lineData>
// 		\ the separator is '|' for the input (bitcoin amounts) and ',' for the database (bitcoin rates)
// 		\ the calls to <validDate>/<validValue> are useless for database file
// 		  since all lines in this file ("data.csv") are valid
int	BitcoinExchange::parseFileLine(std::pair<std::string, double>& lineData, std::string& line, unsigned int isInput)
{
	unsigned long	indSep;
	std::string		dateStr;
	std::string		valueStr;
	double			value;

	if (isInput)
		indSep = line.find('|');
	else
		indSep = line.find(',');
	if (indSep == std::string::npos)
		return (logError("no separator between date and value"));
	if (indSep <= isInput || indSep >= line.length() - 1 - isInput)
		return (logError("data or value field is empty"));
	if (isInput && (line[indSep - 1] != ' ' || line[indSep + 1] != ' '))
		return (logError("no space around separator"));
	dateStr = line.substr(0, indSep - isInput);
	valueStr = line.substr(indSep + 1 + isInput, line.length() - 1);
	if (!(this->isValidDate(dateStr)))
		return (logError("bad date"));
	if (!(this->isValidValue(valueStr)))
		return (logError("not a positive number"));
	value = strtod(valueStr.c_str(), NULL);
	if (isInput && (value < 0 || value > 1000))
		return (logError("too big a number"));
	lineData.first = dateStr;
	lineData.second = value;
	return (0);
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

// Determines if a year is bissextile
// A year is bissextile if and only if it is
// 		- divisible by 4 and not by 100
// 		- divisible by 4 and by 100, but also by 400
bool	isBissextileYear(int year)
{
	if (year % 4 != 0)
		return (0);
	else if (year % 100 != 0)
		return (1);
	else if (year % 400 == 0)
		return (1);
	else
		return (0);
}

// Checks if a character in the digits range of the ASCII table
bool	isDigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

// Checks that a literal representing an int (date number) is valid
// Constraints on string : only digits characters
bool	isValidDateNumber(std::string dateNumberStr)
{
	unsigned int	ind = 0;

	while (ind < dateNumberStr.length())
	{
		if (!isDigit(dateNumberStr[ind]))
			return (0);
		ind++;
	}
	return (1);
}

// Displays the current state of the "date->bitcoinRate" map
void	logMap(std::map<std::string, double>& _map)
{
	std::map<std::string, double>::iterator		it;

	std::cout << "Map :\n";
	if (_map.empty())
	{
		std::cout << "empty\n";
		return ;
	}
	for (it = _map.begin(); it != _map.end(); it++)
		std::cout << "\tkey " << (*it).first << " -> value " << (*it).second << "\n";
}
