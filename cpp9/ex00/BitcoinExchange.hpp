#ifndef BITCOINEXCHANGE_H
# define BITCOINEXCHANGE_H

# include <string>
# include <sstream>
# include <iostream>
# include <fstream>
# include <map>
# include <vector>
# include <algorithm>
# include <stdlib.h>
# include <iomanip>

class	BitcoinExchange
{
	public:
		BitcoinExchange(void);
		BitcoinExchange(std::string recordFileName);
		BitcoinExchange(const BitcoinExchange& orig);
		~BitcoinExchange(void);
		BitcoinExchange&						operator= (const BitcoinExchange& orig);
		const std::map<std::string, double>&	getRecord(void) const;
		int										translateBitcoinAmounts(std::string inputFileName);

	private:
		// Parsing the database file / input file
		int								parseDatabase(std::string recordFileName);
		int								parseFileLine(std::pair<std::string, double>& lineData, std::string& line, unsigned int isInput);
		bool							isValidValue(std::string valueStr);
		bool							isValidDate(std::string dateStr);
		// Answering the translation requests from the input file
		int								fetchBitcoinRateAt(std::string dateStr, std::pair<std::string, double>& closestRecordData);
		void							displayBitcoinTotalValue(std::pair<std::string, double>& amountInfo, std::pair<std::string, double>& rateInfo);
		std::map<std::string, double>	bitcoinRatesRecord;
};

// Utils functions
int		logError(std::string msg);
bool	isBissextileYear(int year);
bool	isDigit(char c);
bool	isValidDateNumber(std::string dateNumberStr);
void	logMap(std::map<std::string, double>& _map);

#endif
