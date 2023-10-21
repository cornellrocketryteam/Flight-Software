#include "flight_mode.hpp"
#include "state.hpp"


void FlightMode::execute() {
    // Poll sensors
}

// Startup Mode

void StartupMode::execute() {
    // Try to init all sensors
}

void StartupMode::transition() {
    if (!state::altimeter::init) {
        // Retry
    }

    if (!state::gps::init) {
        // Retry
    }

    if (!state::imu::init) {
        // Retry
    }

    if (!state::accel::init) {
        // Retry
    }

    if (!state::therm::init) {
        // Retry
    }
    
    if (!state::sd::init) {
        // Retry
    }

    if (!state::rfm::init) {
        // Retry
    }

    state::flight::mode = state::flight::standby;
}

// Standby Mode

void StandbyMode::transition() {
    // Detect acceleration of liftoff

    state::flight::mode = state::flight::powered_ascent;
}


// Powered Ascent Mode

void PoweredAscentMode::transition() {
    // Detect slowing of rocket

    state::flight::mode = state::flight::coasting;
}

// Coasting Mode

void CoastingMode::transition() {
    // Detect apogee

    state::flight::mode = state::flight::drogue_deployed;
}

// Drogue Deployed Mode

void DrogueDeployedMode::transition() {
    // Detect main deployment altitude

    state::flight::mode = state::flight::main_deployed;
}