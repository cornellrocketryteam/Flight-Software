cmake_minimum_required(VERSION 3.21)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(fsw C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
    -Wno-format          
    -Wno-unused-function
)

# TODO: Refactor
add_executable(fsw
    src/main.cpp
)

# Pull in common dependencies
target_link_libraries(fsw pico_stdlib hardware_i2c)

pico_enable_stdio_usb(fsw 1)
pico_enable_stdio_uart(fsw 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(fsw)