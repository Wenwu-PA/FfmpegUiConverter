#include "Menu.h"
#include "ConversionOptions.h"
#include <iostream>
#include <string>
#include <limits>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>

static void fixConsoleUTF8() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
#endif

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

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

static void printHeader() {
    std::cout << BOLD << CYAN
        << "+----------------------------------------------------+\n"
        << "|             FFmpeg Конвертер (профессиональный)    |\n"
        << "|          Поддержка GPU: NVENC, AMF, QSV, AV1, VP9  |\n"
        << "+----------------------------------------------------+\n"
        << RESET;
}

static void printHelp() {
    std::cout << YELLOW << "\n>> Доступные видео-кодеки:\n" << RESET;
    std::cout << "  [CPU]  libx264, libx265, libsvtav1 (AV1), libvpx-vp9 (VP9)\n";
    std::cout << "  [NVIDIA] h264_nvenc, hevc_nvenc, av1_nvenc\n";
    std::cout << "  [AMD]   h264_amf, hevc_amf, av1_amf\n";
    std::cout << "  [Intel] h264_qsv, hevc_qsv\n";
    std::cout << YELLOW << "\n>> Аудио-кодеки: aac, mp3, opus, flac, copy\n" << RESET;
    std::cout << YELLOW << ">> Пресеты (скорость/качество): ultrafast, fast, medium, slow, veryslow\n" << RESET;
    std::cout << GREEN << ">> Подсказка: нажимайте Enter, чтобы оставить значение по умолчанию\n" << RESET;
}

static void printMenu() {
    std::cout << BOLD << "\n================ ГЛАВНОЕ МЕНЮ ================\n" << RESET;
    std::cout << " 1. Конвертировать файл\n";
    std::cout << " 2. Показать историю конвертаций\n";
    std::cout << " 3. Выход\n";
    std::cout << BOLD << "-----------------------------------------------\n" << RESET;
    std::cout << "Ваш выбор: ";
}

static std::string readStringOptional(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static std::string readStringRequired(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty()) return s;
        std::cout << RED << "Ошибка: поле не может быть пустым. Повторите ввод.\n" << RESET;
    }
}

static int readIntOptional(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) return -1;
    try {
        return std::stoi(line);
    }
    catch (...) {
        std::cout << RED << "Не число, будет использовано значение по умолчанию.\n" << RESET;
        return -1;
    }
}

void Menu::run() {
#ifdef _WIN32
    fixConsoleUTF8();
#endif
    printHeader();
    printHelp();

    while (true) {
        printMenu();
        std::string choiceStr;
        std::getline(std::cin, choiceStr);
        if (choiceStr.empty()) continue;
        int choice = std::stoi(choiceStr);

        if (choice == 1) {
            ConversionOptions opt;

            std::cout << CYAN << "\n--- Введите параметры конвертации ---\n" << RESET;

            std::string input = readStringRequired("Входной файл: ");
            std::string output = readStringRequired("Выходной файл: ");
            opt.inputFile = normalizePath(input);
            opt.outputFile = normalizePath(output);

            std::string tmp;
            tmp = readStringOptional("Видео-кодек (default libx264): ");
            if (!tmp.empty()) opt.videoCodec = tmp;

            tmp = readStringOptional("Аудио-кодек (default aac): ");
            if (!tmp.empty()) opt.audioCodec = tmp;

            tmp = readStringOptional("Пресет (default medium): ");
            if (!tmp.empty()) opt.preset = tmp;

            int vbr = readIntOptional("Видео-битрейт (kbps, 0=auto): ");
            if (vbr >= 0) opt.videoBitrate = vbr;

            int abr = readIntOptional("Аудио-битрейт (kbps, 0=auto): ");
            if (abr >= 0) opt.audioBitrate = abr;

            opt.applyDefaults();

            auto err = opt.validate();
            if (err) {
                std::cout << RED << "\nОшибка валидации: " << *err << "\n" << RESET;
                continue;
            }

            std::string cmd = converter.buildCommand(opt);
            std::cout << GREEN << "\n>> Запуск команды:\n" << RESET;
            std::cout << "ffmpeg " << cmd << "\n\n";

            int ret = converter.execute(cmd);
            if (ret != 0) {
                std::cout << RED << "Конвертация завершилась с ошибкой (код " << ret << ")\n" << RESET;
            }
            else {
                std::cout << GREEN << "Конвертация успешно завершена!\n" << RESET;
                history.add("ffmpeg " + cmd);
            }
        }
        else if (choice == 2) {
            auto logs = history.get();
            if (logs.empty()) {
                std::cout << YELLOW << "История пуста.\n" << RESET;
            }
            else {
                std::cout << CYAN << "\n================ ИСТОРИЯ КОНВЕРТАЦИЙ ================\n" << RESET;
                for (size_t i = 0; i < logs.size(); ++i) {
                    std::cout << i + 1 << ". " << logs[i] << "\n";
                }
            }
        }
        else if (choice == 3) {
            std::cout << GREEN << "До свидания!\n" << RESET;
            break;
        }
        else {
            std::cout << RED << "Неверный выбор. Пожалуйста, введите 1, 2 или 3.\n" << RESET;
        }
    }
}