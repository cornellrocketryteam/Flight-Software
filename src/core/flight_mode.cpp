#include "flight_mode.hpp"
#include "../constants.hpp"
#include "../pins.hpp"
#include "hardware/gpio.h"
#include "modules.hpp"
#include "state.hpp"

void FlightMode::execute() {
    bool ret;

    if (!state::alt::status == OFF) {
        ret = modules::altimeter.read_altitude(&state::alt::altitude, state::alt::ref_pressure);
        ret = modules::altimeter.read_pressure(&state::alt::pressure);
        check_sensor(ALT, ret);
    }

    if (!state::imu::status == OFF) {
        ret = modules::imu.read_gyro(
            &state::imu::gyro_x,
            &state::imu::gyro_y,
            &state::imu::gyro_z
        );
        ret = modules::imu.read_mag(
            &state::imu::mag_x,
            &state::imu::mag_y,
            &state::imu::mag_z
        );
        check_sensor(IMU, ret);
    }

    if (!state::accel::status == OFF) {
        ret = modules::accel.read_accel(
            &state::accel::accel_x,
            &state::accel::accel_y,
            &state::accel::accel_z
        );
        check_sensor(ACCEL, ret);
    }

    if (!state::imu::status == OFF) {
        ret = modules::therm.read_temperature(&state::therm::temp);
        ret = modules::therm.read_humidity(&state::therm::humidity);
        check_sensor(THERM, ret);
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

void FlightMode::check_sensor(enum Sensor sensor, bool ret) {
    switch (sensor) {
    case ALT:
        if (ret) {
            if (state::alt::status == INVALID) {
                state::alt::status = VALID;
            }
        } else {
            state::alt::failed_reads++;
            state::alt::status = INVALID;
            state::flight::events.emplace_back("Alt reading failed");
            if (state::alt::failed_reads == constants::flight::max_failed_reads) {
                state::alt::status = OFF;
                state::flight::events.emplace_back("Alt turned off");
            }
        }
        break;
    case IMU:
        if (ret) {
            if (state::imu::status == INVALID) {
                state::imu::status = VALID;
            }
        } else {
            state::imu::failed_reads++;
            state::imu::status = INVALID;
            state::flight::events.emplace_back("IMU reading failed");
            if (state::imu::failed_reads == constants::flight::max_failed_reads) {
                state::imu::status = OFF;
                state::flight::events.emplace_back("IMU turned off");
            }
        }
        break;
    case ACCEL:
        if (ret) {
            if (state::accel::status == INVALID) {
                state::accel::status = VALID;
            }
        } else {
            state::accel::failed_reads++;
            state::accel::status = INVALID;
            state::flight::events.emplace_back("Accel reading failed");
            if (state::accel::failed_reads == constants::flight::max_failed_reads) {
                state::accel::status = OFF;
                state::flight::events.emplace_back("Accel turned off");
            }
        }
        break;
    case THERM:
        if (ret) {
            if (state::therm::status == INVALID) {
                state::therm::status = VALID;
            }
        } else {
            state::therm::failed_reads++;
            state::therm::status = INVALID;
            state::flight::events.emplace_back("Therm reading failed");
            if (state::therm::failed_reads == constants::flight::max_failed_reads) {
                state::therm::status = OFF;
                state::flight::events.emplace_back("Therm turned off");
            }
        }
        break;
    }
}

// Startup Mode

void StartupMode::execute() {

    if (gpio_get(ARMED)) {
        state::flight::key_armed = true;
        state::flight::events.emplace_back("Key armed");
    }
    if (state::alt::status == OFF) {
        if (modules::altimeter.begin()) {
            state::alt::status = VALID;
            // TODO: Investigate the cause of the first reading always being bad
            modules::altimeter.read_pressure(&state::alt::ref_pressure);
            sleep_ms(10);

            modules::altimeter.read_pressure(&state::alt::ref_pressure);
        } else {
            state::flight::events.emplace_back("Alt init failed");
        }
    }
    if (state::imu::status == OFF) {
        if (modules::imu.begin()) {
            state::imu::status = VALID;
        } else {
            state::flight::events.emplace_back("IMU init failed");
        }
    }
    if (state::accel::status == OFF) {
        if (modules::accel.begin()) {
            state::accel::status = VALID;
        } else {
            state::flight::events.emplace_back("Accel init failed");
        }
    }
    if (state::therm::status == OFF) {
        if (modules::therm.begin()) {
            state::therm::status = VALID;
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

void AscentMode::execute() {
    FlightMode::execute();

    // TODO: Change to take in multiple values for arming
    if (state::alt::status == VALID && !state::flight::altitude_armed && state::alt::altitude > constants::flight::arming_altitude) {
        state::flight::altitude_armed = true;
        state::flight::events.emplace_back("Alt armed");
    }
    if (state::flight::altitude_armed && !state::alt::status == OFF) {
        altitude_sum += state::alt::altitude;
        next_alt++;
    }
}

void AscentMode::transition() {
    if (state::flight::altitude_armed && !state::alt::status == OFF && next_alt % 10 == 0) {
        run_filter();
        if (apogee_detected()) {
            gpio_put(SSA_1, 1);
            state::flight::events.emplace_back("Drogue triggered");
            state::flight::mode = state::flight::drogue_deployed;
        }
    }
}

bool AscentMode::apogee_detected() {
    // TODO: Change to an array of filtered alts to support different filtering lengths
    if (filtered_alt1 > filtered_alt2 && filtered_alt2 > filtered_alt3) {
        return true;
    }
    return false;
}

void AscentMode::run_filter() {
    // TODO: Probably refactor
    filtered_alt1 = filtered_alt2;
    filtered_alt2 = filtered_alt3;
    filtered_alt3 = altitude_sum / 10.0;
    altitude_sum = 0;
}

// Drogue Deployed Mode

void DrogueDeployedMode::transition() {
    if (state::alt::altitude < constants::flight::main_deploy_altitude) {
        gpio_put(SSA_2, 1);
        state::flight::events.emplace_back("Main triggered");
        state::flight::mode = state::flight::main_deployed;
    }
}