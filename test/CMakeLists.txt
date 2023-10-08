cmake_minimum_required(VERSION 3.21)

# Pull in SDK (must be before project)
include(../pico_sdk_import.cmake)

project(tests C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
    -Wno-format          
    -Wno-unused-function
)

# Add all subdirectories
add_subdirectory(basic)
