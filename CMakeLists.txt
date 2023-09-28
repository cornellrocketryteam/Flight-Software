cmake_minimum_required(VERSION 3.21)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(crt_fsw C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()


add_compile_options(-Wall
    -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
    -Wno-unused-function # we have some for the docs that aren't called
)

# TODO: Refactor
add_executable(crt_fsw
    src/main.cpp
)

# pull in common dependencies
target_link_libraries(crt_fsw pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(crt_fsw)