#include "PmergeMe.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char **argv) {
    std::vector<unsigned int> *numbers;

    try {
        numbers = InputParser::verifyInput(argc, const_cast<const char **>(argv));
        /* TEST PRINTING
        for (std::vector<unsigned int>::const_iterator nbIt = numbers->begin(); nbIt != numbers->end(); ++nbIt) {
            std::cout << *nbIt << std::endl;
        } */
    } catch (const std::invalid_argument &exception) {
        std::cerr << " Invalid argument has detected " << std::endl;
        std::cerr << exception.what() << std::endl;
        return 1;
    }
    catch (const std::bad_alloc &ignored) {
        std::cerr << " Error => The program has crashed due to bad_alloc" << std::endl;
        return 1;
    }

    PmergeMe pmergeMe = PmergeMe();
    std::vector<std::pair<unsigned int, unsigned> *> *pairs;
    try {
        pairs = pmergeMe.splitInPairs(numbers);
       // pmergeMe.sortPairs(pairs);

        /* //TEST PRINTING
        for (std::vector<std::pair<unsigned int, unsigned int> *>::const_iterator pairsIterator = pairs->begin(); pairsIterator != pairs->end(); ++pairsIterator) {
            std::pair<unsigned int, unsigned int> *pairs = *pairsIterator;
            std::cerr << "{" << pairs->first << "," << pairs->second << "}" << std::endl;
        }
        std::cerr << pmergeMe.getBullyNumber() << std::endl; */

    } catch (const std::invalid_argument &exception) {
        std::cerr << " Error => An error occurred during the 'splitInPairs' " << std::endl;
        std::cerr << exception.what() << std::endl;
        return 1;
    }
    catch (const std::bad_alloc &ignored) {
        std::cerr << " Error => The program has crashed due to bad_alloc" << std::endl;
        return 1;
    }

    return 0;
}
