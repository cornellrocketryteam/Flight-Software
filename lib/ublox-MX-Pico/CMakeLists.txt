add_library(ublox-MX-Pico INTERFACE)

target_sources(ublox-MX-Pico INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/ublox_mx.cpp
)

target_link_libraries(ublox-MX-Pico INTERFACE
    pico_stdlib
    hardware_i2c
)