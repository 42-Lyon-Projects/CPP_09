#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP
#include <string>
#include <ctime>
#include <iostream>
#include <map>

template<typename T>
class Comparator
{
	public:
		static bool compare(T obj, T other)
		{
			return obj > other;
		}

		static bool r_compare(T obj, T other)
		{
			return obj < other;
		}
};

class ParsingDateException : public std::exception {};
class LeapYearDateException : public std::exception {};

class BitcoinDate
{
	private:
		int day, month, years;

	public:
		explicit BitcoinDate(int day = 1, int month = 1, int years = 1);
		explicit BitcoinDate(const std::string &string_date);
		BitcoinDate(const BitcoinDate &other);
		~BitcoinDate();
		BitcoinDate &operator=(const BitcoinDate &other);
		bool operator>(const BitcoinDate &other) const;
		bool operator<(const BitcoinDate &other) const;

		bool date_has_good_format();
		bool is_leap_year() const;
		time_t to_timestamp() const;

		std::string to_string() const;
};

class BitcoinExchange
{
	public:
		std::map<BitcoinDate, float> bitcoins_values;
		explicit BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		~BitcoinExchange();
		BitcoinExchange&operator=(const BitcoinExchange &other);
		std::string* read_btc_xls(const std::string &str) const throw (std::runtime_error);;
		void load_data_from_strings(const std::string str[]);
		float get_btc_value_from_date(BitcoinDate date) const;
		float get_btc_value_from_date(const std::string &date) const;
};

class FileUtils
{
	public:
		static size_t countLines(std::ifstream& fileFlux);
		static void readFromFile(std::ifstream& fileFlux, std::string* lines, size_t linesToRead);
		static void resetIfStream(std::ifstream& fileFlux);
};
#endif
