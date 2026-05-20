#include "CommandParser.h"
#include <sstream>

std::vector<std::string> CommandParser::parse(const std::string& input) {
    std::stringstream ss(input);
    std::vector<std::string> result;
    std::string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}