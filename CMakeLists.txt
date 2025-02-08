cmake_minimum_required(VERSION 3.22)

# Pull in SDK (must be before project)
include(lib/pico-sdk/pico_sdk_init.cmake)

project(fsw C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add compiler flags and definitions
add_compile_options(
    -Wall
    -Wno-format          
    -Wno-unused-function
)

add_definitions(-DVERBOSE)
add_definitions(-DLAUNCH)
# add_definitions(-DSIM)

# Add source files from src/
set(SOURCES
    src/main.cpp
    src/core/fsw.cpp
    src/core/flight_mode.cpp
    src/core/state.cpp
    src/core/modules.cpp
    src/rfm/rfm.cpp
    src/sd/sd.cpp
    src/sd/hw_config.c
    
)

# Add source files from sim/
set(SIM
    sim/sim_data.cpp
)

# Add all executables
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${SIM}
)

# Add a "upload" target that automatically uploads the uf2 to a Pico in boot-select mode
add_custom_target(upload
    COMMAND ${CMAKE_COMMAND} --build . --target all
    COMMAND cp -X ${PROJECT_NAME}.uf2 /Volumes/RPI-RP2/
    DEPENDS ${PROJECT_NAME}
)

# Add external libraries
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/RadioLib)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/BLiMS)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/BMP388-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/BNO055-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/LIS3DH-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/ublox-MX-Pico)

# Ignore warnings from RadioLib
target_compile_options(RadioLib PRIVATE -w)

# Include library headers
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/lib/BMP388-Pico
    ${PROJECT_SOURCE_DIR}/lib/BNO055-Pico
    ${PROJECT_SOURCE_DIR}/lib/LIS3DH-Pico
    ${PROJECT_SOURCE_DIR}/lib/ublox-MX-Pico
    ${PROJECT_SOURCE_DIR}/lib/BLiMS/src
)

# Pull in common dependencies
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib 
    hardware_i2c 
    hardware_pwm
    hardware_gpio
    RadioLib
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico 
    BMP388-Pico
    BNO055-Pico
    LIS3DH-Pico 
    ublox-MX-Pico
    BLiMS
)

# Enable serial over USB
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Create uf2 file
pico_add_uf2_output(${PROJECT_NAME})