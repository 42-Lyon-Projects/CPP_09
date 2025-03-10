#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <string>


class RPN {
private:
    std::list<long> rpnNumbers;
    long lastResult;

public:
    class CalculationException : public std::exception {};
    RPN(void);
    RPN(const RPN &other);
    ~RPN(void);
    RPN &operator=(const RPN &other);

    void addNumber(long number);
    std::pair<long, long> getNumbers();
    void flushNumbers(void);

    long evaluate(const std::string &expression) throw(RPN::CalculationException);
    long getLastResult(void);

};


class InputParser {
private:
    InputParser(void);
    InputParser(const InputParser &other);
    ~InputParser(void);
    InputParser &operator=(const InputParser &other);

public:
    static std::string trim_whitespace(const std::string &str);
    static bool verifyInput(const std::string &input);
};

class RPNUtils {
private:
    RPNUtils(void);
    RPNUtils(const RPNUtils &other);
    ~RPNUtils(void);
    RPNUtils &operator=(const RPNUtils &other);

public:
    static long applyOperand(std::pair<long, long> numbers, char operand) throw (RPN::CalculationException);
    static bool isOperand(const char &c);

};

#endif
