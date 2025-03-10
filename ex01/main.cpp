#include <iostream>

#include "RPN.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <expression>" << std::endl;
        return 1;
    }

    const std::string expression = argv[1];

    if (!InputParser::verifyInput(expression)) {
        std::cout << "An error occurred during the parsing of your expression" << std::endl;
        return 1;
    }

    RPN *firstRPN = new(std::nothrow) RPN();
    if (firstRPN == NULL) {
        std::cout << "Failed to allocate memory for the first RPN object" << std::endl;
        return 1;
    }

    long result = firstRPN->evaluate(expression);
    std::cout << "The result is: " << result << std::endl;

    std::cout << "Last result is: " << firstRPN->getLastResult() << " !" << std::endl;
    //TODO: check if deep copy works well
    delete firstRPN;

    return 0;
}
