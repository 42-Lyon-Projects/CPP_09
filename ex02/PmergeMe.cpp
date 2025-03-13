#include "PmergeMe.hpp"

#include <algorithm>

#include "cstdlib"
#include "cerrno"
#include "cmath"

/***************************************************/
/************* PmergeMe Implementation *************/
/***************************************************/

unsigned int PmergeMe::getBullyNumber() const {
    return this->bullyNumber;
}


/***************************************************/
/*********** InputParser Implementation ************/
/***************************************************/

InputParser::InputParser() {
}

InputParser::~InputParser() {
}

InputParser::InputParser(const InputParser &other) {
    (void) other;
}

InputParser &InputParser::operator=(const InputParser &other) {
    (void) other;
    return *this;
}

std::string InputParser::trim_whitespace(const std::string &str) {
    const std::string fakeRegex = " \t\n\r\f\v";
    const size_t first = str.find_first_not_of(fakeRegex);
    if (first == std::string::npos)
        return str;
    const size_t last = str.find_last_not_of(fakeRegex);
    if (last == std::string::npos)
        return str;
    return str.substr(first, last - first + 1);
}


std::vector<unsigned int> *InputParser::verifyInput(const int argc, const char **argv) throw (std::invalid_argument, std::bad_alloc) {
    if (argc <= 2) throw std::invalid_argument(std::string("Error => ").append(argv[0]).append(" [numbers]"));

    char *endptdr = NULL;
    //TODO: FREE THIS
    std::vector<unsigned int> *numbers = new(std::nothrow) std::vector<unsigned int>();

    if (numbers == NULL)
        throw std::bad_alloc();

    for (int index = 1; argv[index] != NULL; ++index) {
        const std::string nbStr = argv[index];

        if (nbStr.empty()) throw std::invalid_argument(std::string("Error => Empty entry dectected !"));

        const std::string trimmed_input = trim_whitespace(nbStr);

        if (trimmed_input.empty()) throw std::invalid_argument(std::string("Error => Empty entry dectected !"));

        const size_t nb = trimmed_input.find_first_not_of("0123456789");

        if (nb != std::string::npos)
            throw std::invalid_argument(
                std::string("Error => Only positives numbers is allowed !"));

        unsigned int result = strtoul(nbStr.c_str(), &endptdr, 10);

        if (errno != 0 || endptdr == nbStr.c_str()) {
            errno = 0;
            throw std::invalid_argument(std::string("Error => Number convertion failed."));
        }
        numbers->push_back(result);
    }
    return numbers;
}
