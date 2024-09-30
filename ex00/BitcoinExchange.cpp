#include "BitcoinExchange.hpp"
#include <ctime>
#include <cerrno>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <sys/stat.h>

/***************************************************/
/*********** BitcoinDate Implementation ************/
/***************************************************/

BitcoinDate::BitcoinDate(int day, int month, int years)
{
	this->day = day;
	this->month = month;
	this->years = years;
	if (is_leap_year())
		throw LeapYearDateException();
}

BitcoinDate::BitcoinDate(const std::string &string_date)
{
	tm tm;
	if (strptime(string_date.c_str(), "%Y-%m-%d", &tm) == NULL)
		throw ParsingDateException();
	if (errno != 0)
		throw ParsingDateException();

	int years = tm.tm_year + 1900;
	int month = tm.tm_mon + 1;
	int day = tm.tm_mday;
	this->day = day;
	this->month = month;
	this->years = years;
	if (is_leap_year())
		throw LeapYearDateException();
}

BitcoinDate::~BitcoinDate() {}

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
	os << this->years << "-" << month_os.str()  << "-" << this->day;
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

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	std::map<BitcoinDate, float> map = other.bitcoins_values;
	for (std::map<BitcoinDate, float>::iterator it = map.begin(); it != map.end(); ++it)
		this->bitcoins_values.insert(std::pair<BitcoinDate, float>(it->first, it->second));
	map.clear();
}

std::string* BitcoinExchange::read_btc_xls(const std::string &path) const throw (std::runtime_error)
{
	std::ifstream readFileFlux(path.c_str());
	if (!readFileFlux.is_open())
	{
		std::string error = "Unable to find file ";
		error.append(path);
		throw std::runtime_error(error);
	}

	size_t linesAmount = FileUtils::countLines(readFileFlux);
	std::string lines[linesAmount];
	FileUtils::readFromFile(readFileFlux, lines, linesAmount);
	return lines;
}

void load_data_from_strings(const std::string str[])
{
}




/***************************************************/
/************ FileUtils Implementation *************/
/***************************************************/

size_t FileUtils::countLines(std::ifstream &fileFlux)
{
	size_t count = 0;
	std::string line;
	while (!fileFlux.eof()) {
		count++;
		if (!std::getline(fileFlux, line))
			break;
	}
	resetIfStream(fileFlux);
	return count;
}

void FileUtils::readFromFile(std::ifstream &fileFlux, std::string lines[], size_t linesToRead = 0)
{

	for (std::size_t j = 0; j < linesToRead; ++j) {
		std::getline(fileFlux, lines[j]);
	}
	resetIfStream(fileFlux);
}

void FileUtils::resetIfStream(std::ifstream &fileFlux)
{
	fileFlux.clear();
	fileFlux.seekg(0, std::ios::beg);
}