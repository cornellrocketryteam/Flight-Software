#include "flight_mode.hpp"
#include "../constants.hpp"
#include "../pins.hpp"
#include "hardware/gpio.h"
#include "modules.hpp"
#include "state.hpp"
#include <bitset>

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
        check_sensor(IMU, ret);
        ret = modules::imu.read_accel(
            &state::imu::accel_x,
            &state::imu::accel_y,
            &state::imu::accel_z
        );
        check_sensor(IMU, ret);
        ret = modules::imu.read_orientation(
            &state::imu::orientation_x,
            &state::imu::orientation_y,
            &state::imu::orientation_z
        );
        check_sensor(IMU, ret);
        ret = modules::imu.read_gravity(
            &state::imu::gravity_x,
            &state::imu::gravity_y,
            &state::imu::gravity_z
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
        check_sensor(THERM, ret);
    }

    if (state::sd::init) {
        modules::sd.log();
    }

    // if (state::rfm::init) {
    //     modules::rfm.transmit();
    // }

    if (!state::flight::events.empty()) {
        state::flight::events.clear();
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
            state::flight::events.emplace_back(Event::alt_read_fail);
            if (state::alt::failed_reads >= constants::max_failed_reads) {
                state::alt::status = OFF;
                state::flight::events.emplace_back(Event::alt_turn_off);
                to_mode(state::flight::fault);
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
            state::flight::events.emplace_back(Event::imu_read_fail);
            if (state::imu::failed_reads >= constants::max_failed_reads) {
                state::imu::status = OFF;
                state::flight::events.emplace_back(Event::imu_turn_off);
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
            state::flight::events.emplace_back(Event::accel_read_fail);
            if (state::accel::failed_reads >= constants::max_failed_reads) {
                state::accel::status = OFF;
                state::flight::events.emplace_back(Event::accel_turn_off);
                to_mode(state::flight::fault);
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
            state::flight::events.emplace_back(Event::therm_read_fail);
            if (state::therm::failed_reads >= constants::max_failed_reads) {
                state::therm::status = OFF;
                state::flight::events.emplace_back(Event::therm_turn_off);
            }
        }
        break;
    }
}

void FlightMode::to_mode(FlightMode *mode) {
    state::flight::mode = mode;
    if (state::sd::init) {
        modules::sd.write_mode();
    }
}

// Startup Mode

void StartupMode::execute() {
    // Check to see if the arming key has been turned
    if (gpio_get(ARMED_IN)) {
        state::flight::key_armed = true;
        gpio_put(ARMED_OUT, 1);
        state::flight::events.emplace_back(Event::key_armed);
    }

    // Attempt to initialize all modules
    if (state::alt::status == OFF) {
        if (modules::altimeter.begin()) {
            state::alt::status = VALID;
            // The first reading of the BMP388 is always garbage
            modules::altimeter.read_pressure(&state::alt::ref_pressure);
            sleep_ms(10);
            modules::altimeter.read_pressure(&state::alt::ref_pressure);
        } else {
            state::flight::events.emplace_back(Event::alt_init_fail);
        }
    }
    if (state::imu::status == OFF) {
        if (modules::imu.begin()) {
            state::imu::status = VALID;
        } else {
            state::flight::events.emplace_back(Event::imu_init_fail);
        }
    }
    if (state::accel::status == OFF) {
        if (modules::accel.begin()) {
            state::accel::status = VALID;
        } else {
            state::flight::events.emplace_back(Event::accel_init_fail);
        }
    }
    if (state::therm::status == OFF) {
        if (modules::therm.begin()) {
            state::therm::status = VALID;
        } else {
            state::flight::events.emplace_back(Event::therm_init_fail);
        }
    }
    if (!state::sd::init) {
        if (modules::sd.begin()) {
            state::sd::init = true;
        } else {
            state::flight::events.emplace_back(Event::sd_init_fail);
        }
    }
    // if (!state::rfm::init) {
    //     if (modules::rfm.begin()) {
    //         state::rfm::init = true;
    //     } else {
    //         state::flight::events.emplace_back(Event::rfm_init_fail);
    //     }
    // }
}

void StartupMode::transition() {
    // Proceed to Standby Mode if the arming key is turned
    if (state::flight::key_armed) {
        to_mode(state::flight::standby);
    }

    // Proceed to Fault Mode if either flight-critical sensor is non-operational
    if (state::alt::status != VALID || state::accel::status != VALID) {
        to_mode(state::flight::fault);
    }
}

// Standby Mode

void StandbyMode::transition() {
    if (state::accel::status == VALID) {
        filter_accel();

        if (filtered_accel[0] > constants::accel_threshold &&
            filtered_accel[1] > constants::accel_threshold &&
            filtered_accel[2] > constants::accel_threshold) {
            to_mode(state::flight::ascent);
        }
    }
}

void StandbyMode::filter_accel() {
    accel_sum += state::accel::accel_z;
    sample_count++;

    if (sample_count == 10) {
        filtered_accel[2] = filtered_accel[1];
        filtered_accel[1] = filtered_accel[0];
        filtered_accel[0] = accel_sum / 10;
        accel_sum = 0;
        sample_count = 0;
    }
}

// Ascent Mode

void AscentMode::execute() {
    FlightMode::execute();

    // Check for arming altitude
    if (state::alt::status == VALID && !state::flight::alt_armed && state::alt::altitude > constants::arming_altitude) {
        state::flight::alt_armed = true;
        state::flight::events.emplace_back(Event::alt_armed);
    }
}

void AscentMode::transition() {
    // If we're armed and the altimeter is valid, check for apogee
    if (state::flight::alt_armed && state::alt::status == VALID) {
        filter_alt();

        if (filtered_alt[2] > filtered_alt[1] && filtered_alt[1] > filtered_alt[0]) {
            gpio_put(SSA_1, 1);
            state::flight::ematch_start = to_ms_since_boot(get_absolute_time());
            to_mode(state::flight::drogue_deployed);
        }
    }
}

void AscentMode::filter_alt() {
    alt_sum += state::alt::altitude;
    sample_count++;

    if (sample_count == 10) {
        filtered_alt[2] = filtered_alt[1];
        filtered_alt[1] = filtered_alt[0];
        filtered_alt[0] = alt_sum / 10;
        alt_sum = 0;
        sample_count = 0;
    }
}

// Drogue Deployed Mode

void DrogueDeployedMode::execute() {
    // Check to see if we have exceeded the threshold for holding the ematch pin high
    if (to_ms_since_boot(get_absolute_time()) - state::flight::ematch_start >= constants::ematch_threshold) {
        gpio_put(SSA_1, 0);
    }
    FlightMode::execute();
}

void DrogueDeployedMode::transition() {
    // Proceed to Main Deployed mode if the deployment altitude is reached
    if (state::alt::altitude < constants::main_deploy_altitude) {
        gpio_put(SSA_2, 1);
        state::flight::ematch_start = to_ms_since_boot(get_absolute_time());
        to_mode(state::flight::main_deployed);
    }
}

// Main Deployed Mode

void MainDeployedMode::execute() {
    // Check to see if we have exceeded the threshold for holding the ematch pin high
    if (to_ms_since_boot(get_absolute_time()) - state::flight::ematch_start >= constants::ematch_threshold) {
        gpio_put(SSA_2, 0);
    }
    FlightMode::execute();
}