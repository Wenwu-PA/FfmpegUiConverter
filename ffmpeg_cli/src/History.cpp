#include "History.h"

void History::add(const std::string& command) {
    logs.push_back(command);
}

std::vector<std::string> History::get() const {
    return logs;
}