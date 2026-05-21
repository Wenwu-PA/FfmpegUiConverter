#pragma once
#include <vector>
#include <string>

class History {
private:
    std::vector<std::string> logs;

public:
    void add(const std::string& command);
    std::vector<std::string> get() const;
};