#include <gtest/gtest.h>
#include "History.h"

TEST(HistoryTest, AddAndGet) {
    History h;
    h.add("ffmpeg -i in.mp4 out.avi");
    EXPECT_EQ(h.get().size(), 1);
    EXPECT_EQ(h.get()[0], "ffmpeg -i in.mp4 out.avi");
}