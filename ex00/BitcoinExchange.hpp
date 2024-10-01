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


class BitcoinDate
{
	public:
		class InvalidDateException : public std::exception {};

	private:
		int day, month, years;

	public:
		explicit BitcoinDate();
		explicit BitcoinDate(const std::string &string_date) throw (InvalidDateException);
		BitcoinDate(const BitcoinDate &other);
		~BitcoinDate();
		BitcoinDate &operator=(const BitcoinDate &other);
		bool operator>(const BitcoinDate &other) const;
		bool operator<(const BitcoinDate &other) const;

		bool date_has_good_content(const std::string &string) const;
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
		void load_data_from_strings(const std::string *str, size_t lineAmount, char delimiter, std::map<BitcoinDate, float> *map) throw (std::out_of_range, BitcoinDate::InvalidDateException, std::runtime_error);;
		std::pair<time_t, float> find_nearest_pair(const BitcoinDate &date) const;
};

class StringUtils
{
	private:
		StringUtils();
		StringUtils(const StringUtils &other);
		StringUtils &operator=(const StringUtils &other);
		~StringUtils();
	public:
		static std::string trim_whitespace(const std::string &str);
};

class FileUtils
{
	private:
		FileUtils();
		FileUtils(FileUtils const &other);
		FileUtils&operator=(FileUtils const &other);
		~FileUtils();
	public:
		static size_t countLines(std::ifstream& fileFlux);
		static void readFromFile(std::ifstream& fileFlux, std::string* lines, size_t linesToRead);
		static void resetIfStream(std::ifstream& fileFlux);
		static std::string* read_string_array(const std::string &str, size_t *amount) throw (std::runtime_error);

};
#endif
