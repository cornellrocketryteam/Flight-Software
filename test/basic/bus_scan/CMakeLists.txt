add_executable(bus_scan
    bus_scan.c
)

# Pull in common dependencies
target_link_libraries(bus_scan pico_stdlib hardware_i2c)

# Create map/bin/hex file etc.
pico_add_extra_outputs(bus_scan)
