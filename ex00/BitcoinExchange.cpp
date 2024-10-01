#include "BitcoinExchange.hpp"
#include <ctime>
#include <cerrno>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <limits>

/***************************************************/
/*********** BitcoinDate Implementation ************/
/***************************************************/

BitcoinDate::BitcoinDate()
{
	this->day = -1;
	this->month = -1;
	this->years = -1;
}

BitcoinDate::BitcoinDate(const std::string &string_date) throw (InvalidDateException)
{
	tm tm;
	if (strptime(string_date.c_str(), "%Y-%m-%d", &tm) == NULL)
		throw InvalidDateException();
	if (errno != 0)
		throw InvalidDateException();

	int years = tm.tm_year + 1900;
	int month = tm.tm_mon + 1;
	int day = tm.tm_mday;

	this->day = day;
	this->month = month;
	this->years = years;

	if (!date_has_good_content(string_date))
		throw InvalidDateException();
}

BitcoinDate::~BitcoinDate()
{
}

BitcoinDate::BitcoinDate(const BitcoinDate &other)
{
	*this = other;
}

BitcoinDate &BitcoinDate::operator=(const BitcoinDate &other)
{
	this->day = other.day;
	this->month = other.month;
	this->years = other.years;
	return *this;
}

bool BitcoinDate::operator>(const BitcoinDate &other) const
{
	return Comparator<size_t>::compare(this->to_timestamp(), other.to_timestamp());
}

bool BitcoinDate::operator<(const BitcoinDate &other) const
{
	return Comparator<size_t>::r_compare(this->to_timestamp(), other.to_timestamp());
}

bool BitcoinDate::date_has_good_content(const std::string &str) const
{
	for (std::string::size_type i = 0; i < str.size(); ++i)
	{
		const char character = (str[i]);
		if (!std::isdigit(character) && character != '-')
			return false;
	}

	int littleMonth[5] = {2, 4, 6, 9, 11};
	for (int i = 0; i < 4; ++i)
	{
		const int m = littleMonth[i];
		if (month == m)
		{
			if (month == littleMonth[0] && day > 28)
			{
				if (!is_leap_year())
					return false;
				if (day > 29)
					return false;
			}
			if (day > 30)
				return false;
		}
	}

	if (day > 31)
		return false;

	return true;
}

/*
 * @ref: https://fr.wikipedia.org/wiki/Ann%C3%A9e_bissextile
 */
bool BitcoinDate::is_leap_year() const
{
	return (((years % 4 == 0) && (years % 100 != 0)) || years % 400 == 0);
}

std::string BitcoinDate::to_string(void) const
{
	std::ostringstream os;
	std::ostringstream month_os;
	if (this->month > 9)
		month_os << this->month;
	else
		month_os << "0" << this->month;
	os << this->years << "-" << month_os.str() << "-" << this->day;
	return os.str();
}

time_t BitcoinDate::to_timestamp(void) const
{
	tm time;
	time.tm_mday = day;
	time.tm_mon = month - 1;
	time.tm_year = years - 1900;
	time.tm_hour = 12;
	time.tm_min = 30;
	time.tm_sec = 1;
	time.tm_isdst = -1;
	return mktime(&time);
}

/***************************************************/
/********* BitcoinExchange Implementation **********/
/***************************************************/
BitcoinExchange::BitcoinExchange()
{
	this->bitcoins_values = std::map<BitcoinDate, float>();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
{
	*this = other;
}

BitcoinExchange::~BitcoinExchange()
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	std::map<BitcoinDate, float> map = other.bitcoins_values;
	for (std::map<BitcoinDate, float>::iterator it = map.begin(); it != map.end(); ++it)
		this->bitcoins_values.insert(std::pair<BitcoinDate, float>(it->first, it->second));
	map.clear();
	return *this;
}

void BitcoinExchange::load_data_from_strings(const std::string *str, size_t lineAmount, char delimiter, std::map<BitcoinDate, float> *map) throw (std::out_of_range, BitcoinDate::InvalidDateException, std::runtime_error)
{
	for (size_t i = 1; i < lineAmount; ++i)
	{
		const std::string line = str[i];
		const size_t tokenPos = line.find_first_of(delimiter, 0);
		if (tokenPos == std::string::npos)
			continue;

		const std::string date = StringUtils::trim_whitespace(line.substr(0, tokenPos));
		const std::string value = StringUtils::trim_whitespace(line.substr(tokenPos + 1));

		BitcoinDate btc_date = BitcoinDate(date);
		char *end_ptr;
		float btc_value = strtof(value.c_str(), &end_ptr);
		if (end_ptr == value.c_str())
			throw std::runtime_error("Invalid value for BitcoinDate !");
		if (delimiter == '|' && (btc_value > 1000 || btc_value < 0))
			throw std::runtime_error("Invalid value for btc !");
		std::pair<BitcoinDate, float> pair = std::pair<BitcoinDate, float>(btc_date, btc_value);
		map->insert(pair);
	}
}

std::pair<time_t, float> BitcoinExchange::find_nearest_pair(const BitcoinDate &date) const
{
	std::pair<time_t, float> nearest(-1, -1);
	time_t nearest_timestamp = std::numeric_limits<time_t>::min();

	for (std::map<BitcoinDate, float>::const_iterator it = bitcoins_values.begin(); it != bitcoins_values.end(); ++it) {
		time_t timestamp = it->first.to_timestamp();
		if (timestamp <= date.to_timestamp() && timestamp > nearest_timestamp) {
			nearest = std::pair<time_t, float>(date.to_timestamp(), it->second);
			nearest_timestamp = timestamp;
		}
	}
	return nearest;
}


/***************************************************/
/*********** StringUtils Implementation ************/
/***************************************************/

std::string StringUtils::trim_whitespace(const std::string &str)
{
	const std::string fakeRegex = " \t\n\r\f\v";
	size_t first = str.find_first_not_of(fakeRegex);
	if (first == std::string::npos)
		return str;
	size_t last = str.find_last_not_of(fakeRegex);
	return str.substr(first, last - first + 1);
}


/***************************************************/
/************ FileUtils Implementation *************/
/***************************************************/
size_t FileUtils::countLines(std::ifstream &fileFlux)
{
	size_t count = 0;
	std::string line;
	while (!fileFlux.eof())
	{
		if (!std::getline(fileFlux, line))
			break;
		count++;
	}
	resetIfStream(fileFlux);
	return count;
}

void FileUtils::readFromFile(std::ifstream &fileFlux, std::string lines[], size_t linesToRead = 0)
{
	for (std::size_t j = 0; j < linesToRead; ++j)
	{
		std::getline(fileFlux, lines[j]);
	}
	resetIfStream(fileFlux);
}

void FileUtils::resetIfStream(std::ifstream &fileFlux)
{
	fileFlux.clear();
	fileFlux.seekg(0, std::ios::beg);
}

std::string *FileUtils::read_string_array(const std::string &path, size_t *amount) throw (std::runtime_error)
{
	std::ifstream readFileFlux(path.c_str());
	if (!readFileFlux.is_open())
	{
		std::string error = "Unable to find file ";
		error.append(path);
		throw std::runtime_error(error);
	}

	size_t linesAmount = countLines(readFileFlux);
	*amount = linesAmount;
	std::string *lines = new std::string[linesAmount];
	readFromFile(readFileFlux, lines, linesAmount);
	return lines;
}
