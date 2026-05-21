#pragma once
#include <string>

class ICommand {
public:
    virtual void execute() = 0;
    virtual ~ICommand() = default;
};