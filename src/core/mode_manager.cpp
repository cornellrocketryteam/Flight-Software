#include "mode_manager.hpp"
#include "constants.hpp"
#include "modules.hpp"
#include "state.hpp"

class FlightMode;

ModeManager::ModeManager() {
    mode = &startup_class;
}

// TODO Change parameter
void ModeManager::to_mode(int new_mode) {
    switch (new_mode) {
    case 0:
        mode = &startup_class;
        break;
    case 1:
        mode = &standby_class;
    case 2:
        mode = &ascent_class;
    case 3:
        mode = &drogue_deployed_class;
    case 4:
        mode = &main_deployed_class;
    case 5:
        mode = &fault_class;
    default:
        // TODO won't be necessary
        break;
    }

    state->flight.mode_id = new_mode;

    if (state->fram.init) {
        fram.store(Data::old_mode);
    }
}

ModeManager manager;