#include "../Test.h"
#include "ConversionOptions.h"
#include <fstream>
#include <filesystem>

TEST("ConversionOptions: supported codecs and presets")
{
    CONFIRM_TRUE(ConversionOptions::isVideoCodecSupported("libx264"));
    CONFIRM_TRUE(ConversionOptions::isVideoCodecSupported("libsvtav1"));
    CONFIRM_TRUE(ConversionOptions::isVideoCodecSupported("h264_nvenc"));
    CONFIRM_TRUE(ConversionOptions::isAudioCodecSupported("aac"));
    CONFIRM_TRUE(ConversionOptions::isAudioCodecSupported("opus"));
    CONFIRM_TRUE(ConversionOptions::isPresetSupported("libx264", "slow"));
    CONFIRM_TRUE(ConversionOptions::isPresetSupported("libsvtav1", "medium"));
    CONFIRM_TRUE(ConversionOptions::isPresetSupported("h264_nvenc", "p4"));
}

TEST("ConversionOptions: validation fails for missing input file")
{
    ConversionOptions opt;
    opt.inputFile = "nonexistent.mp4";
    opt.outputFile = "out.mp4";
    auto err = opt.validate();
    CONFIRM_TRUE(err.has_value());
    CONFIRM_TRUE(err->find("does not exist") != std::string::npos);
}

TEST("ConversionOptions: validation passes for valid options")
{
    std::ofstream("test_valid.mp4").close();
    ConversionOptions opt;
    opt.inputFile = "test_valid.mp4";
    opt.outputFile = "out.mp4";
    opt.videoCodec = "libx264";
    opt.audioCodec = "aac";
    opt.preset = "fast";
    CONFIRM_FALSE(opt.validate().has_value());
    std::filesystem::remove("test_valid.mp4");
}

TEST("ConversionOptions: validation fails for invalid preset")
{
    std::ofstream("dummy.mp4").close();
    ConversionOptions opt;
    opt.inputFile = "dummy.mp4";
    opt.outputFile = "out.mp4";
    opt.videoCodec = "libx264";
    opt.preset = "invalid_preset";
    auto err = opt.validate();
    CONFIRM_TRUE(err.has_value());
    CONFIRM_TRUE(err->find("Invalid preset") != std::string::npos);
    std::filesystem::remove("dummy.mp4");
}

TEST("ConversionOptions: path with quotes is normalized")
{
    std::ofstream("test_quotes.mp4").close();
    ConversionOptions opt;
    opt.inputFile = "\"test_quotes.mp4\"";
    opt.outputFile = "out.mp4";
    opt.videoCodec = "libx264";
    opt.audioCodec = "aac";
    opt.preset = "fast";
    CONFIRM_FALSE(opt.validate().has_value());
    std::filesystem::remove("test_quotes.mp4");
}