#include <algorithm>
#include "BitcoinExchange.hpp"
#include <dirent.h>
#include <unistd.h>


char *check_variables(int argc) throw (std::invalid_argument) {
    const size_t size = 1024;
    char *buffer = new char[size];

    if (getcwd(buffer, size) == NULL) {
        delete[] buffer;
        throw std::invalid_argument("The current directory is invalid.");
    }
    if (argc != 2) {
        delete[] buffer;
        throw std::invalid_argument("Wrong number of arguments.");
    }

    return buffer;
}


int main(int argc, char **argv) {
    char *buffer = NULL;
    try {
        buffer = check_variables(argc);
    } catch (std::invalid_argument &e) {
        std::cout << "Sorry, the program can't be started !" << std::endl;
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    std::string root_path = buffer;

    std::string *csv_strs = NULL;
    std::string *user_strs = NULL;
    try {
        BitcoinExchange exchange = BitcoinExchange();
        size_t csv_size = 0;
        size_t user_size = 0;

        csv_strs = FileUtils::read_string_array(root_path + "/data.csv", &csv_size);
        std::cout << "Database has " << csv_size << " entries" <<  std::endl;
        exchange.load_data_from_strings(csv_strs, csv_size, ',', &exchange.bitcoins_values);
        std::map<BitcoinDate, float> user_datas = std::map<BitcoinDate, float>();
        user_strs = FileUtils::read_string_array(argv[1], &user_size);
        exchange.load_data_from_strings(user_strs, user_size, '|', &user_datas);
        std::cout << "Input file has " << user_datas.size() << " entries\n" << std::endl;

        for (std::map<BitcoinDate, float>::const_iterator it = user_datas.begin(); it != user_datas.end(); it++) {
            std::pair<time_t, float> finded = exchange.find_nearest_pair(it->first);
            if (finded.first == -1 && finded.second == -1) {
                std::cout << "Unable to find data for the date " << it->first.to_string() << std::endl;
                continue;
            }
            std::cout << it->first.to_string() << " => " << finded.second << " = " << finded.second * it->second << "$" << std::endl;
        }
    } catch (std::out_of_range &ignored) {
        //TODO: FIX IT
        std::cout << "Invalid format inside Xls file, the columns must be separated by comma ','" << std::endl;
    }
    catch (std::runtime_error &exception) {
        std::cout << exception.what() << std::endl;
    }
    if (csv_strs != NULL)
        delete [] csv_strs;
    if (user_strs != NULL)
        delete [] user_strs;
    if (buffer != NULL)
        delete [] buffer;
    return 0;
}
