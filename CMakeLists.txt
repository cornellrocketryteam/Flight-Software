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
    # -O3
)

# Build options
set(VERBOSE OFF)
set(SIM OFF)

# Add build flags and change executable name based on options
if(VERBOSE AND NOT SIM)
    add_definitions(-DVERBOSE)
    set(EXEC_NAME "fsw_verbose")
elseif(NOT VERBOSE AND SIM)
    add_definitions(-DLAUNCH)
    add_definitions(-DSIM)
    set(EXEC_NAME "fsw_sim")
elseif(VERBOSE AND SIM)
    add_definitions(-DVERBOSE)
    add_definitions(-DSIM)
    set(EXEC_NAME "fsw_sim_verbose")
else()
    add_definitions(-DLAUNCH)
    set(EXEC_NAME "fsw")
endif()

# Set custom USB product descriptor
add_definitions(-DUSBD_PRODUCT="FSW Pico")

# Add source files from src/
set(SOURCES
    src/main.cpp
    src/pins.cpp
    src/core/flight_loop.cpp
    src/core/flight_mode.cpp
    src/core/state.cpp
    src/core/modules.cpp
    src/telem/telem.cpp
    src/storage/sd.cpp
    src/storage/fram.cpp
    src/storage/hw_config.cpp
    src/sensor/sensor.cpp
    src/actuator/actuator.cpp
)

# Add source files from sim/
set(SIM_SRC
    sim/sim_data.cpp
    sim/hitl.cpp
)

# Add all executables
add_executable(${EXEC_NAME}
    ${SOURCES}
    ${SIM_SRC}
)

# Add a "upload" target that automatically uploads the uf2 to a Pico in boot-select mode
add_custom_target(upload
    COMMAND ${CMAKE_COMMAND} --build . --target all
    COMMAND cp -X ${EXEC_NAME}.uf2 /Volumes/RPI-RP2/
    DEPENDS ${PROJECT_NAME}
)

# Add external libraries
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/BMP388-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/ublox-MX-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/BNO055-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/LIS3DH-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/ADS1015-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/MB85RS-Pico)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/BLiMS)

# Include project and library headers
target_include_directories(${EXEC_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/sim
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/actuator
    ${PROJECT_SOURCE_DIR}/src/blims
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/sensor
    ${PROJECT_SOURCE_DIR}/src/storage
    ${PROJECT_SOURCE_DIR}/src/telem

    # External library include directories
    ${PROJECT_SOURCE_DIR}/lib/BMP388-Pico
    ${PROJECT_SOURCE_DIR}/lib/ublox-MX-Pico
    ${PROJECT_SOURCE_DIR}/lib/BNO055-Pico
    ${PROJECT_SOURCE_DIR}/lib/LIS3DH-Pico
    ${PROJECT_SOURCE_DIR}/lib/ADS1015-Pico
    ${PROJECT_SOURCE_DIR}/lib/MB85RS-Pico
    ${PROJECT_SOURCE_DIR}/lib/BLiMS/src
)

# Pull in common dependencies
target_link_libraries(${EXEC_NAME} 
    pico_stdlib 
    hardware_adc
    hardware_gpio
    hardware_i2c 
    hardware_pwm
    hardware_uart
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico 
    BMP388-Pico
    ublox-MX-Pico
    BNO055-Pico
    LIS3DH-Pico
    ADS1015-Pico
    MB85RS-Pico
    BLiMS
)

# Enable serial over USB
pico_enable_stdio_usb(${EXEC_NAME} 1)
pico_enable_stdio_uart(${EXEC_NAME} 0)

# Create uf2 file
pico_add_uf2_output(${EXEC_NAME})