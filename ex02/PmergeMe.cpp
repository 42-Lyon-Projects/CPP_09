#include "PmergeMe.hpp"

#include <algorithm>

#include "cstdlib"
#include "cerrno"
#include "cmath"

/***************************************************/
/************* PmergeMe Implementation *************/
/***************************************************/

void PmergeMe::setBullyNumber(const unsigned int nb) {
    this->bullyNumber = nb;
}

unsigned int PmergeMe::getBullyNumber() const {
    return this->bullyNumber;
}


void PmergeMe::destroyVectorOfPair(std::vector<std::pair<unsigned int, unsigned int> *> *vector) {
    if (vector == NULL) return;

    for (std::vector<std::pair<unsigned int, unsigned int> *>::iterator pairsIterator = vector->begin(); pairsIterator != vector->end(); ++pairsIterator) {
        delete *pairsIterator;
    }

    delete vector;
}

std::vector<std::pair<unsigned int, unsigned int> *> *PmergeMe::splitInPairs(std::vector<unsigned int> *numbers) throw (std::invalid_argument, std::bad_alloc) {
    if (numbers == NULL)
        throw std::invalid_argument("The vector passed as argument is null");

    std::vector<std::pair<unsigned int, unsigned int> *> *paired = new(std::nothrow) std::vector<std::pair<unsigned int, unsigned int> *>();

    if (paired == NULL)
        throw std::bad_alloc();

    if (numbers->size() % 2 != 0) {
        this->setBullyNumber(numbers->back());
        numbers->pop_back();
    }

    for (std::vector<unsigned int>::iterator numbersIterator = numbers->begin(); numbersIterator != numbers->end(); ++numbersIterator) {
        unsigned int current = *numbersIterator;
        ++numbersIterator;
        unsigned int next = *numbersIterator;
        std::pair<unsigned int, unsigned int> *pair = new(std::nothrow) std::pair<unsigned int, unsigned int>(current, next);
        if (pair == NULL) {
            destroyVectorOfPair(paired);
            throw std::bad_alloc();
        }

        paired->push_back(pair);
    }
    return paired;
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
