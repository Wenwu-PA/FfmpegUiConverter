#pragma once
#include <string>
#include <optional>

struct ConversionOptions {
    std::string inputFile;
    std::string outputFile;
    std::string videoCodec = "libx264";
    std::string audioCodec = "aac";
    std::string preset = "medium";
    int videoBitrate = 0;   // 0 = default
    int audioBitrate = 128;

    // Проверки поддержки
    static bool isVideoCodecSupported(const std::string& codec);
    static bool isAudioCodecSupported(const std::string& codec);
    static bool isPresetSupported(const std::string& codec, const std::string& preset);
    static bool isOutputFormatSupported(const std::string& outputFile);
    static bool inputFileExists(const std::string& path);

    // Валидация всей структуры
    std::optional<std::string> validate() const;

    // Заполнить недостающие параметры значениями по умолчанию
    void applyDefaults();
};