#include <gtest/gtest.h>
#include "CommandParser.h"

TEST(ParserTest, ParseInput) {
    CommandParser parser;
    auto result = parser.parse("convert a.mp4 b.avi");
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "convert");
    EXPECT_EQ(result[1], "a.mp4");
    EXPECT_EQ(result[2], "b.avi");
}