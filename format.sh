#!/bin/zsh
# Format all C and C++ source files in src/

clang-format -i ./src/*.cpp ./src/*.hpp ./src/**/*.cpp ./src/**/*.hpp ./src/**/*.c ./src/**/*.h