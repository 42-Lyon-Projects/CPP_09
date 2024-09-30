#include <algorithm>

#include "BitcoinExchange.hpp"
#include "vector"


int main(int argc, char **argv) {

	(void) argc;
	(void) argv;

	std::vector<BitcoinDate> v = std::vector<BitcoinDate>();

	BitcoinDate date =  BitcoinDate("2021-02-31");
	BitcoinDate date2 =  BitcoinDate("2019-03-31");
	BitcoinDate date3 =  BitcoinDate("2019-04-31");

	v.push_back(date);
	v.push_back(date2);
	v.push_back(date3);

	std::sort(v.begin(), v.end(), Comparator<BitcoinDate>::r_compare);
	return 0;
}
