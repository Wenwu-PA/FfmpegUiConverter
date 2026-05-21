#include "../Test.h"
#include "FFmpegConverter.h"
#include "ConversionOptions.h"

TEST("FFmpegConverter builds correct command line")
{
    FFmpegConverter conv;
    ConversionOptions opt;
    opt.inputFile = "a.mp4";
    opt.outputFile = "b.avi";
    opt.videoCodec = "libx264";
    opt.audioCodec = "aac";
    opt.preset = "medium";
    opt.videoBitrate = 1000;
    opt.audioBitrate = 128;

    std::string cmd = conv.buildCommand(opt);
    CONFIRM_TRUE(cmd.find("-i \"a.mp4\"") != std::string::npos);
    CONFIRM_TRUE(cmd.find("-c:v libx264") != std::string::npos);
    CONFIRM_TRUE(cmd.find("-c:a aac") != std::string::npos);
    CONFIRM_TRUE(cmd.find("-preset medium") != std::string::npos);
    CONFIRM_TRUE(cmd.find("-b:v 1000k") != std::string::npos);
    CONFIRM_TRUE(cmd.find("-b:a 128k") != std::string::npos);
    CONFIRM_TRUE(cmd.find("-y \"b.avi\"") != std::string::npos);
}

TEST("FFmpegConverter finds ffmpeg path")
{
    std::string path = FFmpegConverter::findFFmpegPath();
    CONFIRM_FALSE(path.empty());
}