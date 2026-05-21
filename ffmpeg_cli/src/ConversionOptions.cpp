#include "ConversionOptions.h"
#include <filesystem>
#include <unordered_set>
#include <algorithm>
#include <cctype>

namespace fs = std::filesystem;

static const std::unordered_set<std::string> videoCodecs = {
    "libx264", "libx265", "libsvtav1", "libvpx-vp9", "copy", "libvpx", "mpeg4",
    "h264_nvenc", "hevc_nvenc", "av1_nvenc",
    "h264_amf", "hevc_amf", "av1_amf",
    "h264_qsv", "hevc_qsv"
};

static const std::unordered_set<std::string> audioCodecs = {
    "aac", "mp3", "copy", "ac3", "opus", "flac", "libopus"
};

static const std::unordered_set<std::string> stringPresets = {
    "ultrafast", "superfast", "veryfast", "faster", "fast",
    "medium", "slow", "slower", "veryslow"
};

static const std::unordered_set<std::string> validExtensions = {
    ".mp4", ".avi", ".mkv", ".mov", ".webm", ".flv", ".m4v"
};

// Нормализация пути – удаляем кавычки и лишние пробелы
static std::string normalizePath(const std::string& path) {
    std::string result = path;
    size_t start = result.find_first_not_of(" \t\n\r");
    if (start != std::string::npos)
        result = result.substr(start);
    size_t end = result.find_last_not_of(" \t\n\r");
    if (end != std::string::npos)
        result = result.substr(0, end + 1);
    if (!result.empty() && result.front() == '"')
        result.erase(0, 1);
    if (!result.empty() && result.back() == '"')
        result.pop_back();
    return result;
}

bool ConversionOptions::isVideoCodecSupported(const std::string& codec) {
    return videoCodecs.find(codec) != videoCodecs.end();
}

bool ConversionOptions::isAudioCodecSupported(const std::string& codec) {
    return audioCodecs.find(codec) != audioCodecs.end();
}

bool ConversionOptions::isPresetSupported(const std::string& codec, const std::string& preset) {
    if (codec == "copy") return true;
    if (codec.find("nvenc") != std::string::npos ||
        codec.find("amf") != std::string::npos ||
        codec.find("qsv") != std::string::npos) {
        return true;
    }
    if (codec == "libsvtav1" || codec == "libvpx-vp9") {
        return true;
    }
    return stringPresets.find(preset) != stringPresets.end();
}

bool ConversionOptions::isOutputFormatSupported(const std::string& outputFile) {
    std::string normalized = normalizePath(outputFile);
    fs::path p(normalized);
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return validExtensions.find(ext) != validExtensions.end();
}

bool ConversionOptions::inputFileExists(const std::string& path) {
    std::string normalized = normalizePath(path);
    return fs::exists(normalized) && !fs::is_directory(normalized);
}

void ConversionOptions::applyDefaults() {
    if (videoCodec.empty()) videoCodec = "libx264";
    if (audioCodec.empty()) audioCodec = "aac";
    if (preset.empty()) preset = "medium";
    if (videoBitrate <= 0) videoBitrate = 0;
    if (audioBitrate <= 0) audioBitrate = 128;
}

std::optional<std::string> ConversionOptions::validate() const {
    if (!inputFileExists(inputFile))
        return "Input file does not exist: " + inputFile;
    if (outputFile.empty())
        return "Output file path is empty";
    if (!isOutputFormatSupported(outputFile))
        return "Unsupported output format. Supported: mp4, avi, mkv, mov, webm, flv, m4v";
    if (!isVideoCodecSupported(videoCodec))
        return "Video codec not supported. Available: libx264, libx265, h264_nvenc, hevc_nvenc, av1_nvenc, h264_amf, hevc_amf, h264_qsv, hevc_qsv, libsvtav1, libvpx-vp9";
    if (!isAudioCodecSupported(audioCodec))
        return "Audio codec not supported. Recommended: aac, mp3, opus, flac";
    if (!isPresetSupported(videoCodec, preset))
        return "Invalid preset. Allowed: ultrafast, superfast, veryfast, faster, fast, medium, slow, slower, veryslow";
    if (videoBitrate < 0 || audioBitrate < 0)
        return "Bitrate cannot be negative";
    return std::nullopt;
}