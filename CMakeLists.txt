cmake_minimum_required(VERSION 3.21)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(fsw C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-DVERBOSE -DDEBUG)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
    -Wno-format          
    -Wno-unused-function
)

# TODO: Refactor
add_executable(fsw
    src/main.cpp
    src/core/fsw.cpp
    src/core/flight_mode.cpp
    src/core/state.cpp
    hw_config.c
)

# Add SD card driver library
add_subdirectory(lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# Pull in common dependencies
target_link_libraries(fsw pico_stdlib hardware_i2c FatFs_SPI)

pico_enable_stdio_usb(fsw 1)
pico_enable_stdio_uart(fsw 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(fsw)