# Flight-Software
Flight software for _Andromeda_, a student-designed hybrid-propellant rocket. Handles parachute deployment, data collection, and telemetry. Also serves as a platform for the [BLiMS](https://github.com/cornellrocketryteam/BLiMS) project.

[Confluence](https://confluence.cornell.edu/display/crt/Flight+Software)

## Getting Started
### Required Dependencies
* ```cmake```
* ```gcc-arm-embedded``` (for macOS)
* ```gcc-arm-none-eabi``` (for Linux)

### Optional Dependencies
* ```doxygen```: For documentation generation
* ```clang-format```: For formatting code

_These dependencies can be install via [Homebrew](https://brew.sh/) on macOS or [apt](https://ubuntu.com/server/docs/package-management) on WSL._
  
### Setup
1. Install the required dependencies
2. Run ```git submodule update --init``` from within ```lib/```
3. Run ```git submodule update --init``` from within ```lib/pico-sdk/```

## Running
1. Create a top-level ```build/``` directory
2. Run ```cmake ..``` from within ```build/```
3. Run ```make```
4. Move ```fsw.uf2``` to a Pico in boot select mode

_Note: You can also run_ ```make upload``` _to automatically make and move_ ```fsw.uf2``` _to a Pico in boot select mode_

## Contributing
1. Create a new branch with ```git checkout -b YOUR_BRANCH_NAME```
2. Make some changes!
3. Make sure to run ```format.sh```
4. Open a new pull request
