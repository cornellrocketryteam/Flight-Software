#include "flight_mode.hpp"
// #include "../../sim/sim_data.hpp"
#include "constants.hpp"
#include "hardware/gpio.h"
#include "pins.hpp"
#include "state.hpp"
// SimData sim_data;

void FlightMode::execute() {
    if (state::alt::status != OFF) {
        altimeter.read_altitude();
    }
    if (state::gps::status != OFF) {
    }
    if (state::imu::status != OFF) {
        imu.read_gyro();
        imu.read_accel();
        imu.read_orientation();
        imu.read_gravity();
    }
    if (state::accel::status != OFF) {
        accel.read_accel();
    }
    if (state::therm::status != OFF) {
        therm.read_temperature();
    }
    if (state::rfm::init) {
        rfm.transmit();
    }
    if (state::sd::init) {
        sd.log();
    }
    if (!state::flight::events.empty()) {
        state::flight::events.clear();
    }
}

void FlightMode::to_mode(FlightMode *mode) {
    state::flight::mode = mode;
    if (state::sd::init) {
        sd.write_mode();
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
        altimeter.begin();
    }
    if (state::gps::status == OFF) {
    }
    if (state::imu::status == OFF) {
        imu.begin();
    }
    if (state::accel::status == OFF) {
        accel.begin();
    }
    if (state::therm::status == OFF) {
        therm.begin();
    }
    if (!state::rfm::attempted_init) {
        rfm.begin();
    }
    if (!state::sd::init) {
        sd.begin();
    }

    // Continuously update reference pressure before launch
    if (state::alt::status == VALID) {
        altimeter.update_ref_pressure();
    }
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

void StandbyMode::execute() {
    FlightMode::execute();

    int c = getchar_timeout_us(0);

    if (c != PICO_ERROR_TIMEOUT) {
        switch ((char)c) {
        case static_cast<char>(Command::launch):
            logf("Command: Launch\n");
            break;
        case static_cast<char>(Command::toggle_mav):
            logf("Command: Toggle MAV\n");
            break;
        case static_cast<char>(Command::toggle_sv):
            logf("Command: Toggle SV\n");
            break;
        case static_cast<char>(Command::clear_card):
            logf("Command: Clear card\n");
            break;
        default:
            state::flight::events.emplace_back(Event::unknown_command);
        }
    }
}

void StandbyMode::transition() {
    to_mode(state::flight::ascent);
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