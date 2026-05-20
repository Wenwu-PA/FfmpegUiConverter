#include <gtest/gtest.h>
#include "ConversionOptions.h"
#include <fstream>
#include <filesystem>

TEST(OptionsTest, SupportedCodecs) {
    EXPECT_TRUE(ConversionOptions::isVideoCodecSupported("libsvtav1"));
    EXPECT_TRUE(ConversionOptions::isVideoCodecSupported("libvpx-vp9"));
    EXPECT_TRUE(ConversionOptions::isVideoCodecSupported("libx265"));
    EXPECT_TRUE(ConversionOptions::isAudioCodecSupported("opus"));
    EXPECT_TRUE(ConversionOptions::isAudioCodecSupported("flac"));

    // isPresetSupported теперь требует кодек + пресет
    EXPECT_TRUE(ConversionOptions::isPresetSupported("libx264", "slow"));
    EXPECT_TRUE(ConversionOptions::isPresetSupported("libsvtav1", "medium"));
    EXPECT_TRUE(ConversionOptions::isPresetSupported("libvpx-vp9", "fast"));
    EXPECT_TRUE(ConversionOptions::isPresetSupported("h264_nvenc", "p4"));
}

TEST(OptionsTest, ValidateMissingInput) {
    ConversionOptions opt;
    opt.inputFile = "nonexistent.mp4";
    opt.outputFile = "out.mp4";
    auto err = opt.validate();
    ASSERT_TRUE(err.has_value());
    EXPECT_NE(err->find("does not exist"), std::string::npos);
}

TEST(OptionsTest, ValidateValid) {
    std::ofstream("test_valid.mp4").close();
    ConversionOptions opt;
    opt.inputFile = "test_valid.mp4";
    opt.outputFile = "out.mp4";
    opt.videoCodec = "libx264";
    opt.audioCodec = "aac";
    opt.preset = "fast";
    EXPECT_FALSE(opt.validate().has_value());
    std::filesystem::remove("test_valid.mp4");
}

TEST(OptionsTest, InvalidPreset) {
    std::ofstream("dummy.mp4").close();
    ConversionOptions opt;
    opt.inputFile = "dummy.mp4";
    opt.outputFile = "out.mp4";
    opt.videoCodec = "libx264";
    opt.preset = "invalid_preset";
    auto err = opt.validate();
    EXPECT_TRUE(err.has_value());
    EXPECT_NE(err->find("Invalid preset"), std::string::npos);
    std::filesystem::remove("dummy.mp4");
}