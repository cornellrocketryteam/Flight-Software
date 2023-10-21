add_executable(simple_sd
    simple_sd.cpp
    ../../../hw_config.c
)

add_subdirectory(../../../lib/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# Pull in common dependencies
target_link_libraries(simple_sd pico_stdlib FatFs_SPI)

# Create map/bin/hex file etc.
pico_add_extra_outputs(simple_sd)
