# Flight-Software
Flight software for the Cornell Rocketry Team.

[Confluence](https://confluence.cornell.edu/display/crt/Flight+Software)

## Getting Started
### Dependencies Required
* ```cmake```
* ```doxygen```
* ```gcc-arm-embedded```
  
### Setup
* Make sure your ```PICO_SDK_PATH``` is set with ```tinyusb``` added as a submodule

## Running
1. Create a top-level ```build``` directory
2. Run ```cmake ..``` from within ```build```
3. Run ```make```
4. Move ```fsw.uf2``` to a Pico in boot select mode


## Contributing
