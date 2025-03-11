#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include "iostream"
#include "vector"


class PmergeMe {
private:
    //Si le nombre de chiffre pass2 en argument nest pas pair je prend le dernier et je le met dans bully number afin d'avoir un nombre d'elements pairs
    unsigned int bullyNumber;

public:
    //Fonction qui met le bully number de cot2 et prends tous les nombres du vecteur pour les mettres dans des paires.
    std::vector<std::pair<unsigned int, unsigned int> *> *splitInPairs(std::vector<unsigned int> *numbers) throw (std::invalid_argument, std::bad_alloc);

    //Fonction qui va trier les paires afin d'avoir les nombres les plus grands tout le temps a droite de la paire.

    template<typename Container>
    void sortPairs(Container<std::pair<unsigned int, unsigned int> *> *pairsContainers) {
        typename Container::iterator pairsIterator = pairsContainers->begin();

        for (; pairsIterator != pairsIterator->end(); ++pairsIterator) {
            std::pair<unsigned int, unsigned int> *pair = *pairsIterator;
            if (pair->first > pair->second)
                std::swap(pair->first, pair->second);
        }
    }

    void setBullyNumber(const unsigned int nb);

    unsigned int getBullyNumber() const;

    //Fonction utilitaire permettant de supprimer les paires allou2es dans un vecteur allou2
    void destroyVectorOfPair(std::vector<std::pair<unsigned int, unsigned int> *> *vector);
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
