cmake_minimum_required(VERSION 3.10)

set(CMAKE_TOOLCHAIN_FILE
    "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "")

project(FfmpegUiConverter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest CONFIG REQUIRED)

include_directories(include)

add_library(core
    src/CommandParser.cpp
    src/History.cpp
    src/FFmpegConverter.cpp
    src/ConversionOptions.cpp
)

add_executable(app
    src/main.cpp
    src/CLI.cpp
)

target_link_libraries(app PRIVATE core)

add_executable(test_runner
    tests/test_converter.cpp
    tests/test_parser.cpp
    tests/test_history.cpp
    tests/test_options.cpp
)

target_link_libraries(test_runner PRIVATE core GTest::gtest GTest::gtest_main)