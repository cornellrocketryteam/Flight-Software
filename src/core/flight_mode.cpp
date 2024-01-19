#include "flight_mode.hpp"
#include "../constants.hpp"
#include "../pins.hpp"
#include "hardware/gpio.h"
#include "modules.hpp"
#include "state.hpp"

void FlightMode::execute() {
    bool ret;
    double x, y, z;

    if (state::alt::init) {
        ret = modules::altimeter.read_altitude(&state::alt::altitude, constants::alt::ref_pressure);
        ret = modules::altimeter.read_pressure(&state::alt::pressure);
    }

    if (state::imu::init) {
        ret = modules::imu.read_gyro(&x, &y, &z);
        state::imu::gyro_x = x;
        state::imu::gyro_y = y;
        state::imu::gyro_z = z;

        ret = modules::imu.read_mag(&x, &y, &z);
        state::imu::mag_x = x;
        state::imu::mag_y = y;
        state::imu::mag_z = z;
    }

    if (state::accel::init) {
        ret = modules::accel.read_accel(&x, &y, &z);
        state::accel::accel_x = x;
        state::accel::accel_y = y;
        state::accel::accel_z = z;
    }

    if (state::therm::init) {
        modules::therm.read_temperature(&state::therm::temp);
        modules::therm.read_humidity(&state::therm::humidity);
    }

    if (!state::flight::altitude_armed && state::alt::altitude > constants::flight::arming_altitude) {
        state::flight::altitude_armed = true;
        // TODO: Log event
    }

    if (state::sd::init) {
        modules::sd.log();
    }

    if (state::rfm::init) {
        modules::rfm.transmit();
    }
}

// Startup Mode

void StartupMode::execute() {

    if (gpio_get(ARMED)) {
        state::flight::key_armed = true;
        // TODO: Log event
    }
    if (!state::alt::init) {
        if (modules::altimeter.begin()) {
            state::alt::init = true;
        } else {
            // TODO: Log failure
        }
    }
    if (!state::imu::init) {
        if (modules::imu.begin()) {
            state::imu::init = true;
        } else {
            // TODO: Log failure
        }
    }
    if (!state::accel::init) {
        if (modules::accel.begin()) {
            state::accel::init = true;
        } else {
            // TODO: Log failure
        }
    }
    if (!state::therm::init) {
        if (modules::therm.begin()) {
            state::therm::init = true;
        } else {
            // TODO: Log failure
        }
    }
    if (!state::sd::init) {
        if (modules::sd.begin()) {
            state::sd::init = true;
        } else {
            // TODO: Log failure
        }
    }
    // if (!state::rfm::init) {
    //     if (modules::rfm.begin()) {
    //         state::rfm::init = true;
    //     } else {
    //         // TODO: Log failure
    //     }
    // }
}

void StartupMode::transition() {
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
    // TODO: Detect deceleration of rocket

    state::flight::mode = state::flight::coasting;
}

// Coasting Mode

void CoastingMode::transition() {
    if (apogee_detected()) {
        // TODO: Log event
        gpio_put(SSA_1, 1);
        state::flight::mode = state::flight::drogue_deployed;
    }
}

bool CoastingMode::apogee_detected() {
    if (!state::flight::altitude_armed) {
        return false;
    }
    // TODO: Port apogee detection from old FSW
    return false;
}

// Drogue Deployed Mode

void DrogueDeployedMode::transition() {
    if (state::alt::altitude < constants::flight::main_deploy_altitude) {
        // TODO: Log event
        gpio_put(SSA_2, 1);
        state::flight::mode = state::flight::main_deployed;
    }
}