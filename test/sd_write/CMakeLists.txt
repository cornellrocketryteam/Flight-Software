cmake_minimum_required(VERSION 3.27)

# Pull in SDK (must be before project)
include(../../lib/pico_sdk_init.cmake)

project(sd_write C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

add_executable(sd_write
    sd_write.cpp
    ../../src/sd/hw_config.c
)

add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)


# Pull in common dependencies
target_link_libraries(sd_write pico_stdlib FatFs_SPI)

pico_enable_stdio_usb(sd_write 1)
pico_enable_stdio_uart(sd_write 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(sd_write)