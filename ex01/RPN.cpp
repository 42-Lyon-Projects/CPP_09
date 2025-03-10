#include "RPN.hpp"

#include <iostream>

RPN::RPN() {
    this->rpnNumbers = std::list<long>();
    this->lastResult = 0;
}

RPN::~RPN() {
    this->flushNumbers();
}

RPN::RPN(const RPN &other) {
    *this = other;
}

RPN &RPN::operator=(const RPN &other) {
    if (this == &other)
        return *this;

    std::list<long>::const_iterator numbersIterator = other.rpnNumbers.begin();
    while (numbersIterator != this->rpnNumbers.end()) {
        this->rpnNumbers.push_back(*numbersIterator);
        ++numbersIterator;
    }

    this->lastResult = other.lastResult;
    return *this;
}


void RPN::addNumber(long number) {
    this->rpnNumbers.push_back(number);
}

std::pair<long, long> RPN::getNumbers() {
    long last = this->rpnNumbers.back();
    this->rpnNumbers.pop_back();
    long newLast = this->rpnNumbers.back();
    this->rpnNumbers.pop_back();
    return std::make_pair(last, newLast);
}

void RPN::flushNumbers() {
    this->rpnNumbers.clear();
}

long RPN::evaluate(const std::string &expression) throw (RPN::CalculationException) {
    std::string::const_iterator expressionIterator = expression.begin();
    long result = 0;
    while (expressionIterator != expression.end()) {
        if (std::isspace(*expressionIterator)) {
            ++expressionIterator;
            continue;
        }

        if (std::isdigit(*expressionIterator)) {
            long number = *(expressionIterator) - '0';
            addNumber(number);
        } else if (RPNUtils::isOperand(*expressionIterator)) {
            std::pair<long, long> numbers = getNumbers();
            try {
                result = RPNUtils::applyOperand(numbers, *expressionIterator);
                addNumber(result);
            }
            catch (RPN::CalculationException &e) {
                std::cout << "Unable to solve this expression: Check if you not divide per 0 " << std::endl;
            }
        }
        ++expressionIterator;
    }

    return result;
}

long RPN::getLastResult() {
    return this->lastResult;
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


bool InputParser::verifyInput(const std::string &input) {
    if (input.empty())
        return false;

    const std::string trimmed_input = trim_whitespace(input);

    if (trimmed_input.empty())
        return false;

    std::string::const_iterator iterator = trimmed_input.begin();
    while (iterator != trimmed_input.end()) {
        const char currentChar = *iterator;
        if (!std::isdigit(currentChar) && !RPNUtils::isOperand(currentChar) && !std::isspace(currentChar)) {
            /**  std::cout << "Invalid input : " << std::endl;
              std::cout << input << std::endl;
              std::cout << std::isdigit(currentChar) << std::endl;
              std::cout << RPNUtils::isOperand(currentChar) << std::endl;
              std::cout << isspace(currentChar) << std::endl;**/

            return false;
        }
        ++iterator;
    }
    return true;
}


/***************************************************/
/************ RPNUtils Implementation **************/
/***************************************************/

RPNUtils::RPNUtils() {
}

RPNUtils::~RPNUtils() {
}

RPNUtils::RPNUtils(const RPNUtils &other) {
    (void) other;
}

RPNUtils &RPNUtils::operator=(const RPNUtils &other) {
    (void) other;
    return *this;
}

long RPNUtils::applyOperand(std::pair<long, long> numbers, char operand) throw (RPN::CalculationException) {
    if (operand == '-') return numbers.second - numbers.first;
    if (operand == '+') return numbers.second + numbers.first;
    if (operand == '*') return numbers.second * numbers.first;
    if (operand == '/') {
        if (numbers.first == 0)
            throw RPN::CalculationException();
        return numbers.second / numbers.first;
    }
    return 0;
}

bool RPNUtils::isOperand(const char &c) {
    return c == '-' || c == '+' || c == '*' || c == '/';
}
