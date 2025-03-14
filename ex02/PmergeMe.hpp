#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <algorithm>

#include "iostream"
#include <vector>
#include <deque>
#include <tgmath.h>


class PmergeMe {
private:
    //Si le nombre de chiffre pass2 en argument nest pas pair je prend le dernier et je le met dans bully number afin d'avoir un nombre d'elements pairs
    unsigned int bullyNumber;

    template<typename Container>
    void sort(Container *container) {
        if (container == NULL) {
            throw std::invalid_argument("The vector passed as argument is null");
        }

        if (container->size() % 2 != 0) {
            this->bullyNumber = container->back();
            container->pop_back();
        }

        splitInPairs(container);
    }


    template<typename Iterator>
    void swapIterators(Iterator beginToMove, Iterator insert) {
       // std::cout << "Distance between iterators: " << std::distance(beginToMove, insert) << std::endl;
        for (Iterator it = beginToMove; it != insert; ++it) {
            std::iter_swap(it, insert);
           // std::cout << "Swap effectué" << std::endl;
        }
    }


    template<typename Container>
void splitInPairs(Container *numbers) {
        typename Container::iterator iterator;

        int pairsDepth = 1;
        unsigned int contentByPair = 0;
        int maxPairs = 10;

        while (pairsDepth < maxPairs) {
            contentByPair = pow(2, pairsDepth);
            maxPairs = numbers->size() / contentByPair;

            std::cout << "-----------------------------------------------------" << std::endl;
            std::cout << "Content per Pair: " << contentByPair << " elements" << std::endl;
            std::cout << "NbSize: " << numbers->size() << std::endl;
            std::cout << "Pair Will Maked: " << maxPairs << std::endl;
            std::cout << "Pair Depth: " << pairsDepth << '\n' << std::endl;

            for (iterator = numbers->begin(); std::distance(iterator, numbers->end()) >= contentByPair; std::advance(iterator, contentByPair)) {
                typename Container::iterator startingOfCurrentPair = iterator;
                typename Container::iterator endOfCurrentpair = iterator + contentByPair - 1;

                std::cout << "Starting Pair with: " << *startingOfCurrentPair << " End Pair with " << *endOfCurrentpair << std::endl;


                if (pairsDepth > 1) {
                    typename Container::iterator pairedEnd = iterator + (contentByPair - pairsDepth) -1;
                    std::cout << "Previous End : " << *pairedEnd << " End Pair with " << *endOfCurrentpair << std::endl;

                    if (*pairedEnd > *endOfCurrentpair) {
                        //Partir du debut de la pair a swap et la swp
                        swapIterators(pairedEnd-pairsDepth/2, endOfCurrentpair);
                    }

                }

                if (*startingOfCurrentPair > *endOfCurrentpair) {
                    swapIterators(startingOfCurrentPair, endOfCurrentpair);
                }


            }
            printPairs(numbers, contentByPair);
            ++pairsDepth;
        }
       // printPairs(numbers);
    }

    template<typename Container>

    void printPairs(Container *pairs, int amountInPair) {
        std::cout << "[" << std::flush;
        int i = 0;
        for (typename std::vector<unsigned int>::const_iterator pairContent = pairs->begin(); pairContent != pairs->end(); ++pairContent) {
            if (i == amountInPair) {
                std::cout << "]" << std::endl;
                std::cout << "[" << std::flush;
                i = 0;
            }
            std::cout << *pairContent << std::flush;
            if (pairContent != pairs->end() - 1)
                std::cout << "," << std::flush;
            ++i;
        }
        std::cout << "] " << std::endl;
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
