#pragma once
#include <vector>
#include <string>

class CommandParser {
public:
    std::vector<std::string> parse(const std::string& input);
};