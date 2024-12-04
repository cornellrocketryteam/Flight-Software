cmake_minimum_required(VERSION 3.27)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(gps C CXX ASM)
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
    poll_data.cpp
    ../ublox_mx.cpp
)
# Pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_i2c
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create uf2 file
pico_add_uf2_output(${PROJECT_NAME})