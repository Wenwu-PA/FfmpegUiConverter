#pragma once
#include <string>
#include "ConversionOptions.h"

class FFmpegConverter {
public:
    // Поиск пути к ffmpeg (переменная окружения FFMPEG_PATH или в PATH)
    static std::string findFFmpegPath();

    // Построение командной строки ffmpeg из структуры ConversionOptions
    std::string buildCommand(const ConversionOptions& options) const;

    // Выполнение готовой команды
    int execute(const std::string& command) const;
};