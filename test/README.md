# Tests

This directory contains unit tests of components that are integrated into flight software.

## Running

You can choose to build any number of these targets with CMake. For example, to run only the ```bus_scan``` target located in ```basic/bus_scan```, run the following in this directory:

```zsh
cmake -S . -B ./build
```
to generate the build system for this directory in a ```build``` directory. Then, you can run

```zsh
cmake --build ./build --target bus_scan
```
to build the ```bus_scan``` target. To build every target in the ```test/``` directory, simply omit the ```--target``` flag.

## Contributing

When adding a new test, make sure to ```add_subdirectory``` in the CMakeLists files!
