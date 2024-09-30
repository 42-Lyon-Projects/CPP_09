#include "BitcoinExchange.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

BitcoinDate::BitcoinDate(int day, int month, int years)
{
	this->day = day;
	this->month = month;
	this->years = years;
}

BitcoinDate::BitcoinDate(const std::string &string_date)
{
	tm tm;
	strptime(string_date.c_str(), "%Y-%m-%d", &tm);
	int years = tm.tm_year + 1900;
	int month = tm.tm_mon + 1;
	int day = tm.tm_mday;
	*this = BitcoinDate(years, month, day);
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

/*
 * @ref: https://fr.wikipedia.org/wiki/Ann%C3%A9e_bissextile
 */
bool BitcoinDate::isLeapYear() const
{
	return (((years % 4 == 0) && (years % 100 != 0)) || years % 400 == 0);
}

std::string BitcoinDate::toString(void) const
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

time_t BitcoinDate::toTimestamp(void) const
{
	tm time;
	time.tm_mday = day;
	time.tm_mon = month - 1;
	time.tm_year = years - 100;
	return mktime(&time);
}

bool BitcoinDate::compare(BitcoinDate other_date) const
{
	return this->toTimestamp() > other_date.toTimestamp();
}


BitcoinDate *BitcoinExchange::parseDate(const std::string &str)
{
	return new (std::nothrow) BitcoinDate(str);
}
