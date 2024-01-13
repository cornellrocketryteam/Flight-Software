cmake_minimum_required(VERSION 3.22)

# Pull in SDK (must be before project)
include(../../lib/pico-sdk/pico_sdk_init.cmake)

project(rf_rx C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

add_executable(rf_rx
    rf_rx.cpp
)

add_subdirectory("../../lib/RadioLib" "${CMAKE_CURRENT_BINARY_DIR}/RadioLib")

# Pull in common dependencies
target_link_libraries(rf_rx pico_stdlib hardware_spi hardware_gpio hardware_timer RadioLib)


pico_enable_stdio_usb(rf_rx 1)
pico_enable_stdio_uart(rf_rx 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(rf_rx)