#include "flight_mode.hpp"
// #include "../../sim/sim_data.hpp"
#include "../constants.hpp"
#include "../pins.hpp"
#include "hardware/gpio.h"
#include "modules.hpp"
#include "state.hpp"
// SimData sim_data;

void FlightMode::execute() {
    if (!state::alt::status == OFF) {
        ret = modules::altimeter.read_data(&state::alt::altitude, &state::alt::temp, state::alt::ref_pressure);
        ret = modules::altimeter.read_pressure(&state::alt::pressure);
        check_sensor(ALT);
    }
    // state::alt::altitude = sim_data.get_alt();

    if (!state::gps::status == OFF) {
        ret = modules::gps.read_data(&state::gps::data);
        check_sensor(GPS);
    }

    if (!state::imu::status == OFF) {
        ret = modules::imu.read_gyro(
            &state::imu::gyro_x,
            &state::imu::gyro_y,
            &state::imu::gyro_z
        );
        check_sensor(IMU);
        ret = modules::imu.read_accel(
            &state::imu::accel_x,
            &state::imu::accel_y,
            &state::imu::accel_z
        );
        check_sensor(IMU);
        ret = modules::imu.read_orientation(
            &state::imu::orientation_x,
            &state::imu::orientation_y,
            &state::imu::orientation_z
        );
        check_sensor(IMU);
        ret = modules::imu.read_gravity(
            &state::imu::gravity_x,
            &state::imu::gravity_y,
            &state::imu::gravity_z
        );
        check_sensor(IMU);
    }

    if (!state::accel::status == OFF) {
        ret = modules::accel.read_accel(
            &state::accel::accel_x,
            &state::accel::accel_y,
            &state::accel::accel_z
        );
        check_sensor(ACCEL);
    }
    // state::accel::accel_z = sim_data.get_accel();

    if (state::rfm::init) {
        modules::rfm.transmit();
    }

    if (state::sd::init) {
        modules::sd.log();
    }

    if (!state::flight::events.empty()) {
        state::flight::events.clear();
    }
}

void FlightMode::check_sensor(enum Sensor sensor) {
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
    case GPS:
        if (ret) {
            if (state::gps::status == INVALID) {
                state::gps::status = VALID;
            }
        } else {
            state::gps::failed_reads++;
            state::gps::status = INVALID;
            state::flight::events.emplace_back(Event::gps_read_fail);
            // if (state::gps::failed_reads >= 100) {
            //     state::gps::status = OFF;
            //     state::flight::events.emplace_back(Event::gps_turn_off);
            // }
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
    }

    // Attempt to initialize all modules
    if (state::alt::status == OFF) {
        if (modules::altimeter.begin()) {
            state::alt::status = VALID;
            // The first reading of the BMP388 is always garbage
            modules::altimeter.read_pressure(&state::alt::ref_pressure);
            sleep_ms(100);
            modules::altimeter.read_pressure(&state::alt::ref_pressure);
        } else {
            if (!state::alt::failed_init) {
                state::flight::events.emplace_back(Event::alt_init_fail);
                state::alt::failed_init = true;
            }
        }
    }
    if (state::alt::status == VALID) {
        modules::altimeter.read_pressure(&pressure);
        state::alt::ref_pressure = alpha * pressure + (1 - alpha) * state::alt::ref_pressure;
    }
    if (state::gps::status == OFF) {
        if (modules::gps.begin()) {
            state::gps::status = VALID;
        } else {
            if (!state::gps::failed_init) {
                state::flight::events.emplace_back(Event::gps_init_fail);
                state::gps::failed_init = true;
            }
        }
    }
    if (state::imu::status == OFF) {
        if (modules::imu.begin()) {
            state::imu::status = VALID;
        } else {
            if (!state::imu::failed_init) {
                state::flight::events.emplace_back(Event::imu_init_fail);
                state::imu::failed_init = true;
            }
        }
    }
    if (state::accel::status == OFF) {
        if (modules::accel.begin()) {
            state::accel::status = VALID;
        } else {
            if (!state::accel::failed_init) {
                state::flight::events.emplace_back(Event::accel_init_fail);
                state::accel::failed_init = true;
            }
        }
    }
    if (!state::rfm::attempted_init) {
        state::rfm::attempted_init = true;
        if (modules::rfm.begin()) {
            state::rfm::init = true;
        } else {
            state::flight::events.emplace_back(Event::rfm_init_fail);
        }
    }
    if (!state::sd::init) {
        if (modules::sd.begin()) {
            state::sd::init = true;
        } else {
            if (!state::sd::failed_init) {
                state::flight::events.emplace_back(Event::sd_init_fail);
                state::sd::failed_init = true;
            }
        }
    }
    modules::blims.pwm_setup();
}

void StartupMode::transition() {
    if (state::flight::old_mode > 1) {
        // Proceed to Fault Mode if we were mid-flight or faulted in the last boot
        to_mode(state::flight::fault);
    } else if (state::flight::key_armed) {
        if (state::alt::status != VALID || state::accel::status != VALID) {
            // Proceed to Fault Mode if either flight-critical sensor is non-operational
            to_mode(state::flight::fault);
        } else {
            // Proceed to Standby Mode otherwise
            to_mode(state::flight::standby);
        }
    }
}

// Standby Mode

void StandbyMode::transition() {
    if (state::accel::status == VALID) {
        accel_sum -= accel_buffer[index];
        accel_buffer[index] = state::accel::accel_z;
        accel_sum += state::accel::accel_z;
        index++;
        if (index == 10) {
            index = 0;
        }

        if (accel_sum * 0.1 > constants::accel_threshold) {
            to_mode(state::flight::ascent);
        }
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
    if (state::alt::status == VALID && state::flight::alt_armed) {
        // If we're armed and the altimeter is valid, start checking for apogee
        alt_sum -= alt_buffer[index];
        alt_buffer[index] = state::alt::altitude;
        alt_sum += state::alt::altitude;
        index++;
        if (index == 10) {
            index = 0;
        }

        if (count == interval) {
            filtered_alt[2] = filtered_alt[1];
            filtered_alt[1] = filtered_alt[0];
            filtered_alt[0] = alt_sum * 0.1;
            count = 0;
        }
        count++;

        if (filtered_alt[2] != -1 && filtered_alt[1] != -1 && filtered_alt[0] != -1 &&
            filtered_alt[2] > filtered_alt[1] && filtered_alt[1] > filtered_alt[0]) {
            gpio_put(SSA_DROGUE, 1);
            state::flight::ematch_start = to_ms_since_boot(get_absolute_time());
            to_mode(state::flight::drogue_deployed);
        }
    }
}

// Drogue Deployed Mode

void DrogueDeployedMode::execute() {
    // Check to see if we have exceeded the threshold for holding the ematch pin high
    if (to_ms_since_boot(get_absolute_time()) - state::flight::ematch_start >= constants::ematch_threshold) {
        gpio_put(SSA_DROGUE, 0);
    }
    FlightMode::execute();
}

void DrogueDeployedMode::transition() {
    // Proceed to Main Deployed mode if the deployment altitude is reached and we have waited for main_deploy_wait
    if (main_cycle_count < constants::main_deploy_wait) {
        main_cycle_count++;
    } else if (main_cycle_count == constants::main_deploy_wait) {
        state::flight::events.emplace_back(Event::main_deploy_wait_end);
        main_cycle_count++;
    } else if (state::alt::altitude < constants::main_deploy_altitude) {
        gpio_put(SSA_MAIN, 1);
        state::flight::ematch_start = to_ms_since_boot(get_absolute_time());
        add_alarm_in_ms(constants::initial_hold_threshold, BLIMS::execute, NULL, true);
        to_mode(state::flight::main_deployed);
    }
}

// Main Deployed Mode

void MainDeployedMode::execute() {
    // Check to see if we have exceeded the threshold for holding the ematch pin high
    if (to_ms_since_boot(get_absolute_time()) - state::flight::ematch_start >= constants::ematch_threshold) {
        gpio_put(SSA_MAIN, 0);
    }
    if (log_cycle_count < constants::main_log_shutoff) {
        log_cycle_count++;
    }
    // Turn off data logging after a certain period of time to not overwrite data after landing
    if (log_cycle_count == constants::main_log_shutoff) {
        state::sd::init = false;
        state::flight::events.emplace_back(Event::main_log_shutoff);
        log_cycle_count++;
    }
    FlightMode::execute();
}