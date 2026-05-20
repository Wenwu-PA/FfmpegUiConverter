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

bool ConversionOptions::isVideoCodecSupported(const std::string& codec) {
    return videoCodecs.find(codec) != videoCodecs.end();
}

bool ConversionOptions::isAudioCodecSupported(const std::string& codec) {
    return audioCodecs.find(codec) != audioCodecs.end();
}

// РЕАЛИЗАЦИЯ isPresetSupported (была пропущена)
bool ConversionOptions::isPresetSupported(const std::string& codec, const std::string& preset) {
    if (codec == "copy") return true;
    // GPU-кодеки принимают любые строки (преобразуем позже)
    if (codec.find("nvenc") != std::string::npos ||
        codec.find("amf") != std::string::npos ||
        codec.find("qsv") != std::string::npos) {
        return true;
    }
    // AV1 и VP9 тоже принимают любые строки (преобразуем в число)
    if (codec == "libsvtav1" || codec == "libvpx-vp9") {
        return true;
    }
    // Для остальных (x264, x265) проверяем по списку строковых пресетов
    return stringPresets.find(preset) != stringPresets.end();
}

bool ConversionOptions::isOutputFormatSupported(const std::string& outputFile) {
    fs::path p(outputFile);
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return validExtensions.find(ext) != validExtensions.end();
}

bool ConversionOptions::inputFileExists(const std::string& path) {
    return fs::exists(path) && !fs::is_directory(path);
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