# Flight-Software
Flight software for _Andromeda_, a student-designed hybrid-propellant rocket. Handles parachute deployment, valve actuation, data collection, and telemetry. Also serves as a platform for the [BLiMS](https://github.com/cornellrocketryteam/BLiMS) project.

[Confluence](https://confluence.cornell.edu/display/crt/Flight+Software)

## Getting Started
### Required dependencies
* ```cmake```
* ```gcc-arm-embedded``` (for macOS)
* ```gcc-arm-none-eabi``` (for Linux)

### Optional dependencies
* ```doxygen```: For documentation generation
* ```clang-format```: For formatting code

_These dependencies can be install via [Homebrew](https://brew.sh/) on macOS or [apt](https://ubuntu.com/server/docs/package-management) on WSL._
  
### Setup
1. Install the required dependencies
2. Run ```git submodule update --init``` from within ```lib/```
3. Run ```git submodule update --init``` from within ```lib/pico-sdk/```

## Running
### Build options
By default, FSW builds in ```LAUNCH``` mode. There are 2 build options you can set: 
* ```VERBOSE```: Displays state information over serial. FSW will only start running once a serial connection is made. 
* ```SIM```: FSW will execute a simulated flight path once the launch command is sent.

You can set these options in ```CMakeLists.txt```. The executable name will change based on which build option(s) are set: ```fsw```, ```fsw_verbose```, ```fsw_sim```, or ```fsw_sim_verbose```.

### Building

1. _(First-time setup only)_ Create a top-level ```build/``` directory    
2. _(First-time setup only)_ Run ```cmake ..``` from within ```build/```  
3. Set build options (see above)
4. Run ```make```
5. Move the generated ```uf2``` file to a Pico in boot select mode

_You can also run_ ```make upload``` _to automatically make and move the uf2 to a Pico in boot select mode_

## Contributing
1. Create a new branch with ```git checkout -b YOUR_BRANCH_NAME```
2. Make some changes!
3. Make sure to run ```format.sh```
4. Open a new pull request
