#include "flight_mode.hpp"
#include "state.hpp"
#include "hardware/gpio.h"


void FlightMode::execute() {
    float x, y, z;

    state::accel::accel.read_accel(&x, &y, &z);

    state::accel::accel_x = x;
    state::accel::accel_y = y;
    state::accel::accel_z = z;

    state::therm::temp = state::therm::therm.read_temperature();
    state::therm::humidity = state::therm::therm.read_humidity();
}

// Startup Mode

void StartupMode::execute() {

    if (gpio_get(6)) {
        state::flight::key_armed = true;
    }

    if (state::accel::accel.begin()) {
        state::accel::init = true;
    }

    if (state::therm::therm.begin()) {
        state::therm::init = true;
    }

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

    if (state::flight::key_armed) {
        state::flight::mode = state::flight::standby;
    }
}

// Standby Mode

void StandbyMode::transition() {
    // TODO: Detect acceleration of liftoff

    state::flight::mode = state::flight::powered_ascent;
}


// Powered Ascent Mode

void PoweredAscentMode::transition() {
    // TODO: Detect slowing of rocket

    state::flight::mode = state::flight::coasting;
}

// Coasting Mode

void CoastingMode::transition() {
    if (apogee_detected()) {
        deploy_drogue();
        state::flight::mode = state::flight::drogue_deployed;
    }
}

bool CoastingMode::apogee_detected() {
    if (!state::flight::armed) {
        return false;
    }
    // TODO: Port apogee prediction from old FSW
}

void CoastingMode::deploy_drogue() {
    // TODO: Send drogue deploy trigger. Maybe this doesn't need to be its own method?
}

// Drogue Deployed Mode

void DrogueDeployedMode::transition() {
    // Detect main deployment altitude

    state::flight::mode = state::flight::main_deployed;
}