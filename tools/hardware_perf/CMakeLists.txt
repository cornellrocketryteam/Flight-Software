cmake_minimum_required(VERSION 3.22)

# Pull in SDK (must be before project)
include(../../lib/pico-sdk/pico_sdk_init.cmake)

project(hardware_perf C CXX ASM)
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
    ${PROJECT_NAME}.cpp
    ${PROJECT_SOURCE_DIR}/../../src/pins.cpp
    ${PROJECT_SOURCE_DIR}/../../src/storage/hw_config.cpp
)

add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/BMP388-Pico ${CMAKE_BINARY_DIR}/BMP388-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/ublox-MX-Pico ${CMAKE_BINARY_DIR}/ublox-MX-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/BNO055-Pico ${CMAKE_BINARY_DIR}/BNO055-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/LIS3DH-Pico ${CMAKE_BINARY_DIR}/LIS3DH-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/ADS1015-Pico ${CMAKE_BINARY_DIR}/ADS1015-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/../../lib/MB85RS-Pico ${CMAKE_BINARY_DIR}/MB85RS-Pico)

# Include project and library headers
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/../../src/
    # External library include directories
    ${PROJECT_SOURCE_DIR}/../../lib/BMP388-Pico
    ${PROJECT_SOURCE_DIR}/../../lib/ublox-MX-Pico
    ${PROJECT_SOURCE_DIR}/../../lib/BNO055-Pico
    ${PROJECT_SOURCE_DIR}/../../lib/LIS3DH-Pico
    ${PROJECT_SOURCE_DIR}/../../lib/ADS1015-Pico
    ${PROJECT_SOURCE_DIR}/../../lib/MB85RS-Pico
)

# Pull in common dependencies
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_adc
    hardware_i2c
    hardware_spi
    hardware_gpio
    hardware_pwm
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico 
    BMP388-Pico
    ublox-MX-Pico
    BNO055-Pico
    LIS3DH-Pico
    ADS1015-Pico
    MB85RS-Pico
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create uf2 file
pico_add_uf2_output(${PROJECT_NAME})