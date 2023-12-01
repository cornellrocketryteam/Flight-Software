cmake_minimum_required(VERSION 3.27)

# Pull in SDK (must be before project)
include(../../pico_sdk_import.cmake)

project(detect_armed C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

add_executable(detect_armed
    detect_armed.cpp
)

# Pull in common dependencies
target_link_libraries(detect_armed pico_stdlib hardware_gpio)

pico_enable_stdio_usb(detect_armed 1)
pico_enable_stdio_uart(detect_armed 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(detect_armed)