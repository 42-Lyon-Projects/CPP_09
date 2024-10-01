#include <algorithm>
#include "BitcoinExchange.hpp"
#include <dirent.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	const size_t size = 1024;
	char buffer[size];

	if (getcwd(buffer, size) == NULL)
	{
		std::cerr << "Sorry, i'm unable to find the current directory " << std::endl;
		return EXIT_FAILURE;
	}

	if (argc != 2)
	{
		std::cerr << "Sorry, you need to pass a file as parameters !" << std::endl;
		return EXIT_FAILURE;
	}

	std::string root_path = buffer;

	std::string *csv_strs = NULL;
	std::string *user_strs = NULL;
	try
	{
		BitcoinExchange exchange = BitcoinExchange();
		size_t csv_size = 0;
		size_t user_size = 0;

		csv_strs = FileUtils::read_string_array(root_path + "/data.csv", &csv_size);
		exchange.load_data_from_strings(csv_strs, csv_size, ',', &exchange.bitcoins_values);
		std::map<BitcoinDate, float> user_datas = std::map<BitcoinDate, float>();
		user_strs = FileUtils::read_string_array(argv[1], &user_size);
		exchange.load_data_from_strings(user_strs, user_size, '|',&user_datas);
		std::cout << user_datas.size() << std::endl;

		for (std::map<BitcoinDate, float>::const_iterator it = user_datas.begin(); it != user_datas.end(); it++)
		{
			std::pair<time_t, float> finded = exchange.find_nearest_pair(it->first);
			if (finded.first == -1 && finded.second == -1)
			{
				std::cout << "Unable to find data for the date " << it->first.to_string() << std::endl;
				continue;
			}
			std::cout << "The nearest date of " << it->first.to_string() << " is " << finded.first << '\n' << std::flush;
			std::cout << "The value of btc is " << finded.second << '\n' << std::flush;
			std::cout << "Conversion to dollars is equals to " << finded.second * it->second << "$" << '\n' << std::endl;

		}
	}
	catch (std::out_of_range &ignored)
	{
		std::cout << "Invalid format inside Xls file, the columns must be separated by comma ','" << std::endl;
	}
	catch (BitcoinDate::InvalidDateException &exception)
	{
		std::cout << "A Invalid BitcoinDate inside Csv file" << std::endl;
	}
	catch (std::runtime_error &exception)
	{
		std::cout << exception.what() << std::endl;
	}
	if (csv_strs != NULL)
		delete [] csv_strs;
	if (user_strs != NULL)
		delete [] user_strs;
	return 0;
}
