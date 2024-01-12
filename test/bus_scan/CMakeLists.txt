cmake_minimum_required(VERSION 3.21)

# Pull in SDK (must be before project)
include(../../lib/pico-sdk/pico_sdk_init.cmake)

project(bus_scan C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

add_executable(bus_scan
    bus_scan.cpp
)

# Pull in common dependencies
target_link_libraries(bus_scan pico_stdlib hardware_i2c)

pico_enable_stdio_usb(bus_scan 1)
pico_enable_stdio_uart(bus_scan 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(bus_scan)