#define _CRT_SECURE_NO_WARNINGS

#include "FFmpegConverter.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif

namespace fs = std::filesystem;

static bool isInPath(const std::string& program) {
#ifdef _WIN32
    char pathBuf[MAX_PATH];
    if (SearchPathA(NULL, program.c_str(), ".exe", MAX_PATH, pathBuf, NULL) != 0) {
        return true;
    }
    return false;
#else
    std::string cmd = "which " + program + " > /dev/null 2>&1";
    return std::system(cmd.c_str()) == 0;
#endif
}

static std::string findInPath(const std::string& program) {
#ifdef _WIN32
    char pathBuf[MAX_PATH];
    if (SearchPathA(NULL, program.c_str(), ".exe", MAX_PATH, pathBuf, NULL) != 0) {
        return std::string(pathBuf);
    }
    return "";
#else
    std::string cmd = "which " + program;
    std::string result;
    char buffer[256];
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    if (!result.empty()) {
        result.pop_back();
        return result;
    }
    return "";
#endif
}

std::string FFmpegConverter::findFFmpegPath() {
    // 1. Переменная окружения FFMPEG_PATH
    const char* envPath = std::getenv("FFMPEG_PATH");
    if (envPath && fs::exists(envPath)) {
        return envPath;
    }

    // 2. Локальные папки (рядом с программой)
    const char* localPaths[] = { "./ffmpeg", "./ffmpeg.exe", "ffmpeg/ffmpeg.exe" };
    for (const char* p : localPaths) {
        if (fs::exists(p)) {
            return p;
        }
    }

    // 3. Поиск в системном PATH
    if (isInPath("ffmpeg")) {
        return findInPath("ffmpeg");
    }

    // 4. Не найден
    std::cerr << "Warning: ffmpeg not found in PATH, local folder, or FFMPEG_PATH.\n";
    std::cerr << "Please install ffmpeg or set FFMPEG_PATH environment variable.\n";
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