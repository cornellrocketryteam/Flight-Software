cmake_minimum_required(VERSION 3.22)

# Pull in SDK (must be before project)
include(../../lib/pico-sdk/pico_sdk_init.cmake)

project(clear_card C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

add_executable(${PROJECT_NAME}
    clear_card.cpp
    ../../src/sd/hw_config.c
)

add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)


# Pull in common dependencies
target_link_libraries(${PROJECT_NAME} pico_stdlib hardware_gpio FatFs_SPI)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})