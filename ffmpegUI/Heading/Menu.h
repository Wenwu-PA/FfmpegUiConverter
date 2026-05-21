#pragma once
#include "CommandParser.h"
#include "FFmpegConverter.h"
#include "History.h"

class Menu {
private:
    CommandParser parser;
    FFmpegConverter converter;
    History history;

public:
    void run();
};