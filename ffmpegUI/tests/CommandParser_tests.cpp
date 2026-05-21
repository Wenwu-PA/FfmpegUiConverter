#include "../Test.h"
#include "CommandParser.h"

TEST("CommandParser splits simple string")
{
    CommandParser parser;
    auto res = parser.parse("convert a.mp4 b.avi");
    CONFIRM(3, (int)res.size());
    CONFIRM("convert", res[0]);
    CONFIRM("a.mp4", res[1]);
    CONFIRM("b.avi", res[2]);
}