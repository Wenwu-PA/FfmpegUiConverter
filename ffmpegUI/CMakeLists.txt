cmake_minimum_required(VERSION 3.10)
project(FfmpegUiConverter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Heading)

add_library(core
    Body/CommandParser.cpp
    Body/History.cpp
    Body/FFmpegConverter.cpp
    Body/ConversionOptions.cpp
)

add_executable(app
    Body/main.cpp
    Body/Menu.cpp
)

target_link_libraries(app PRIVATE core)

add_executable(test_runner
    tests/main.cpp
    tests/FFmpegConverter_tests.cpp
    tests/CommandParser_tests.cpp
    tests/History_tests.cpp
    tests/ConversionOptions_tests.cpp
)

target_link_libraries(test_runner PRIVATE core)
target_include_directories(test_runner PRIVATE tests)

if(MSVC)
    target_compile_options(test_runner PRIVATE /std:c++20)
else()
    target_compile_options(test_runner PRIVATE -std:c++20)
endif()