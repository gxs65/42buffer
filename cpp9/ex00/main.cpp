#include <string>
#include <iostream>
#include "BitcoinExchange.hpp"

int	main(int ac, char **av)
{
	BitcoinExchange		bitcoinTranslator("data.csv");

	if (ac == 2)
		bitcoinTranslator.translateBitcoinAmounts(std::string(av[1]));
	else
		bitcoinTranslator.translateBitcoinAmounts("input.txt");

	return (0);
}