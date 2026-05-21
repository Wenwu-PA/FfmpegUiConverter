cmake_minimum_required(VERSION 3.10)
project(FfmpegUiConverter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем заголовки
include_directories(include)

# Библиотека core – основная логика
add_library(core
    src/CommandParser.cpp
    src/History.cpp
    src/FFmpegConverter.cpp
    src/ConversionOptions.cpp
)

# Основное приложение
add_executable(app src/main.cpp src/CLI.cpp)
target_link_libraries(app PRIVATE core)

# Тестовый runner – все тесты для MereTDD
add_executable(test_runner
    tests/main.cpp
    tests/FFmpegConverter_tests.cpp
    tests/CommandParser_tests.cpp
    tests/History_tests.cpp
    tests/ConversionOptions_tests.cpp
)
target_link_libraries(test_runner PRIVATE core)

# Для MSVC нужен флаг /std:c++20, для GCC/Clang -std=c++20
if(MSVC)
    target_compile_options(test_runner PRIVATE /std:c++20)
else()
    target_compile_options(test_runner PRIVATE -std=c++20)
endif()