#define _CRT_SECURE_NO_WARNINGS  // отключает предупреждение о getenv

#include "FFmpegConverter.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

std::string FFmpegConverter::findFFmpegPath() {
    const char* envPath = std::getenv("FFMPEG_PATH");
    if (envPath && fs::exists(envPath))
        return envPath;
    const char* systemPaths[] = { "ffmpeg", "ffmpeg.exe" };
    for (const char* p : systemPaths) {
        if (fs::exists(p))
            return p;
    }
    return "ffmpeg";
}

static std::string quote(const std::string& s) {
    return "\"" + s + "\"";
}

static int av1PresetToInt(const std::string& preset) {
    if (preset == "ultrafast") return 0;
    if (preset == "superfast") return 1;
    if (preset == "veryfast")  return 2;
    if (preset == "faster")    return 3;
    if (preset == "fast")      return 4;
    if (preset == "medium")    return 5;
    if (preset == "slow")      return 6;
    if (preset == "slower")    return 7;
    if (preset == "veryslow")  return 8;
    return 5;
}

static int vp9PresetToCpuUsed(const std::string& preset) {
    if (preset == "ultrafast" || preset == "superfast") return 5;
    if (preset == "veryfast" || preset == "faster")     return 4;
    if (preset == "fast")      return 3;
    if (preset == "medium")    return 2;
    if (preset == "slow")      return 1;
    if (preset == "slower" || preset == "veryslow") return 0;
    return 2;
}

static int gpuPresetToValue(const std::string& preset) {
    if (preset == "ultrafast" || preset == "superfast") return 1;
    if (preset == "veryfast" || preset == "faster")     return 2;
    if (preset == "fast")      return 3;
    if (preset == "medium")    return 4;
    if (preset == "slow")      return 5;
    if (preset == "slower" || preset == "veryslow") return 6;
    return 4;
}

std::string FFmpegConverter::buildCommand(const ConversionOptions& opt) const {
    std::string cmd;
    cmd += "-i " + quote(opt.inputFile);
    cmd += " -c:v " + opt.videoCodec;
    cmd += " -c:a " + opt.audioCodec;

    if (opt.videoCodec != "copy") {
        if (opt.videoCodec == "libsvtav1") {
            cmd += " -preset " + std::to_string(av1PresetToInt(opt.preset));
        }
        else if (opt.videoCodec == "libvpx-vp9") {
            cmd += " -cpu-used " + std::to_string(vp9PresetToCpuUsed(opt.preset));
        }
        else if (opt.videoCodec.find("nvenc") != std::string::npos ||
            opt.videoCodec.find("amf") != std::string::npos) {
            cmd += " -preset p" + std::to_string(gpuPresetToValue(opt.preset));
        }
        else if (opt.videoCodec.find("qsv") != std::string::npos) {
            cmd += " -preset " + opt.preset;
        }
        else {
            cmd += " -preset " + opt.preset;
        }
    }

    if (opt.videoBitrate > 0)
        cmd += " -b:v " + std::to_string(opt.videoBitrate) + "k";
    if (opt.audioBitrate > 0)
        cmd += " -b:a " + std::to_string(opt.audioBitrate) + "k";

    cmd += " -y " + quote(opt.outputFile);
    return cmd;
}

int FFmpegConverter::execute(const std::string& command) const {
    std::string ffmpeg = findFFmpegPath();
    std::string full = ffmpeg + " " + command;
    return std::system(full.c_str());
}