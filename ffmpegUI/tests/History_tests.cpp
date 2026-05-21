#include "../Test.h"
#include "History.h"

TEST("History stores commands")
{
    History h;
    h.add("ffmpeg -i in.mp4 out.avi");
    CONFIRM(1, (int)h.get().size());
    CONFIRM("ffmpeg -i in.mp4 out.avi", h.get()[0]);
}

TEST("History stores multiple commands")
{
    History h;
    h.add("cmd1");
    h.add("cmd2");
    CONFIRM(2, (int)h.get().size());
    CONFIRM("cmd1", h.get()[0]);
    CONFIRM("cmd2", h.get()[1]);
}