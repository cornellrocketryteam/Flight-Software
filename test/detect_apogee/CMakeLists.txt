cmake_minimum_required(VERSION 3.22)

project(detect_apogee C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

add_executable(${PROJECT_NAME}
    detect_apogee.cpp
)