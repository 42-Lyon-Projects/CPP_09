#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <algorithm>

#include "iostream"
#include <vector>
#include <deque>


class PmergeMe {
private:
    //Si le nombre de chiffre pass2 en argument nest pas pair je prend le dernier et je le met dans bully number afin d'avoir un nombre d'elements pairs
    unsigned int bullyNumber;

    template<typename Container>
    void sort(Container container) {
        if (container == NULL) {
            throw std::invalid_argument("The vector passed as argument is null");
        }

        if (container->size() % 2 != 0) {
            this->bullyNumber = container->back();
            container->pop_back();
        }

        splitInPairs(container, 1);
    }

template<typename Container>
void swapIterators(Container *container, typename Container::iterator *beginToMove, int amountToMove, typename Container::iterator *insert) {
    (void) container;
    typename Container::iterator begin = *beginToMove;
    typename Container::iterator insertPos = *insert;
    int distance = std::distance(begin, insertPos);

    // Si la distance est négative, cela signifie que insert est avant beginToMove
    if (distance < 0) {
        distance = -distance;
        std::rotate(begin, begin + amountToMove, begin + distance);
    } else {
        std::rotate(begin, begin + amountToMove, begin + distance + amountToMove);
    }
}

template<typename Container>
void splitInPairs(Container *numbers, int pairsDepth) {
    typename Container::iterator iterator;

    const bool canPair = pairsDepth == 1 ? numbers->size() % pairsDepth == 0 : numbers->size() / pairsDepth % pairsDepth == 0;
    int pairLimit = (canPair == 1 ? (pairsDepth == 1 ? numbers->size() : numbers->size() / pairsDepth) : (numbers->size() / pairsDepth) - !(numbers->size() % pairsDepth));
    pairLimit = pairLimit / 2;

    if (pairLimit == 1) return;

    std::cout << "" << std::endl;
    std::cout << "NbSize: " << numbers->size() << std::endl;
    std::cout << "Can Pair: " << canPair << std::endl;
    std::cout << "Pair Limit: " << pairLimit << std::endl;
    std::cout << "Pair Depth: " << pairsDepth << '\n' << std::endl;

    for (iterator = numbers->begin(); std::distance(iterator, numbers->end()) >= 2; std::advance(iterator, 2)) {
        const unsigned int current = *iterator;
        typename Container::iterator nextIt = iterator + (1 * pairsDepth);
        const unsigned int next = *nextIt;

        if (current > next) {
            swapIterators(numbers, &iterator, 1 * pairsDepth, &nextIt);
        }

        for (typename Container::iterator it = iterator; it != nextIt; ++it) {
            std::cout << *it << " " << std::flush;
        }
        std::cout << std::endl;

    }

    if (!canPair) {
        return;
    }
    splitInPairs(numbers, ++pairsDepth);
}

public:
    template<typename Container>
    void sortVector(std::vector<unsigned int> *nb) {
        sort(nb);
    }

    template<typename Container>
    void sortDeque(std::deque<unsigned int> *nb) {
        sort(nb);
    }

    unsigned int getBullyNumber() const;
};


class InputParser {
private:
    InputParser(void);

    InputParser(const InputParser &other);

    ~InputParser(void);

    InputParser &operator=(const InputParser &other);

public:
    static std::string trim_whitespace(const std::string &str);

    static std::vector<unsigned int> *verifyInput(const int argc, const char **argv) throw (std::invalid_argument, std::bad_alloc);
};


#endif
