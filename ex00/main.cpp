#include <iostream>

#include "BitcoinExchange.hpp"

int main() {
	BitcoinExchange value = BitcoinExchange();
	BitcoinDate *date = value.parseDate("2021-02-31");
	BitcoinDate *date2 = value.parseDate("2024-02-31");

	std::cout << date->toString() << std::endl;
	std::cout << date2->toString() << std::endl;

	std::cout << date->compare(*date2) << std::endl;

	return 0;
}
