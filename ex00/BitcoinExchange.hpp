#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <ctime>

class BitcoinDate
{
	private:
		int day, month, years;
	public:
		explicit BitcoinDate(int day = 1, int month = 1, int years = 1);
		explicit BitcoinDate(const std::string &string_date);
		~BitcoinDate();
		BitcoinDate(const BitcoinDate &other);
		BitcoinDate&operator=(const BitcoinDate &other);
		bool isLeapYear() const;
		std::string toString(void) const;
		time_t toTimestamp(void) const;
		bool compare(BitcoinDate other_date) const;
};

class BitcoinExchange {
	public:
		BitcoinDate *parseDate(const std::string &str);
};

#endif
