# Flight-Software
Flight software for the Cornell Rocketry Team.

[Confluence](https://confluence.cornell.edu/display/crt/Flight+Software)

## Getting Started
### Dependencies Required
* ```cmake```
* ```doxygen```
* ```gcc-arm-embedded``` (for macOS)
* ```gcc-arm-none-eabi``` (for Linux)
  
### Setup
* Install the required dependencies
* Run ```git submodule update --init``` from within ```lib/```
* Run ```git submodule update --init``` from within ```lib/pico-sdk/```

## Running
1. Create a top-level ```build/``` directory
2. Run ```cmake ..``` from within ```build/```
3. Run ```make```
4. Move ```fsw.uf2``` to a Pico in boot select mode

## Contributing
1. Create a new branch with ```git checkout -b YOUR_BRANCH_NAME```
2. Make some changes!
3. Make sure to run ```format.sh```
4. Open a new pull request
