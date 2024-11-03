cmake_minimum_required(VERSION 3.22)

# Pull in SDK (must be before project)
include(../../lib/pico-sdk/pico_sdk_init.cmake)

project(blims_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

add_executable(blims_test
    blims_test.cpp
)


# Pull in common dependencies
target_link_libraries(blims_test pico_stdlib hardware_pwm hardware_gpio hardware_timer)


pico_enable_stdio_usb(blims_test 1)
pico_enable_stdio_uart(blims_test 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(blims_test)