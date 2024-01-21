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

    // TODO: Move this
    if (!state::flight::altitude_armed && state::alt::altitude > constants::flight::arming_altitude) {
        state::flight::altitude_armed = true;
        state::flight::events.emplace_back("Alt armed");
    }

    if (state::sd::init) {
        modules::sd.log();
    }

    if (!state::flight::events.empty()) {
        state::flight::events.clear();
    }

    if (state::rfm::init) {
        modules::rfm.transmit();
    }
}

// Startup Mode

void StartupMode::execute() {

    if (gpio_get(ARMED)) {
        state::flight::key_armed = true;
        state::flight::events.emplace_back("Key armed");
    }
    if (!state::alt::init) {
        if (modules::altimeter.begin()) {
            state::alt::init = true;
        } else {
            state::flight::events.emplace_back("Alt init failed");
        }
    }
    if (!state::imu::init) {
        if (modules::imu.begin()) {
            state::imu::init = true;
        } else {
            state::flight::events.emplace_back("IMU init failed");
        }
    }
    if (!state::accel::init) {
        if (modules::accel.begin()) {
            state::accel::init = true;
        } else {
            state::flight::events.emplace_back("Accel init failed");
        }
    }
    if (!state::therm::init) {
        if (modules::therm.begin()) {
            state::therm::init = true;
        } else {
            state::flight::events.emplace_back("Therm init failed");
        }
    }
    if (!state::sd::init) {
        if (modules::sd.begin()) {
            state::sd::init = true;
        } else {
            state::flight::events.emplace_back("SD init failed");
        }
    }
    if (!state::rfm::init) {
        if (modules::rfm.begin()) {
            state::rfm::init = true;
        } else {
            state::flight::events.emplace_back("RFM init failed");
        }
    }
}

void StartupMode::transition() {
    if (state::flight::key_armed) {
        state::flight::mode = state::flight::standby;
    }
}

// Standby Mode

void StandbyMode::transition() {
    // TODO: Detect acceleration of liftoff

    state::flight::mode = state::flight::ascent;
}

// Ascent Mode

void AscentMode::transition() {
    if (apogee_detected()) {
        gpio_put(SSA_1, 1);
        state::flight::events.emplace_back("Drogue triggered");
        state::flight::mode = state::flight::drogue_deployed;
    }
}

bool AscentMode::apogee_detected() {
    if (!state::flight::altitude_armed) {
        return false;
    }
    // TODO: Port apogee detection from old FSW
    return false;
}

// Drogue Deployed Mode

void DrogueDeployedMode::transition() {
    if (state::alt::altitude < constants::flight::main_deploy_altitude) {
        gpio_put(SSA_2, 1);
        state::flight::events.emplace_back("Main triggered");
        state::flight::mode = state::flight::main_deployed;
    }
}