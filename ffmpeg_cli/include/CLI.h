#pragma once
#include "CommandParser.h"
#include "FFmpegConverter.h"
#include "History.h"

class CLI {
private:
    CommandParser parser;
    FFmpegConverter converter;
    History history;

public:
    void run();
};