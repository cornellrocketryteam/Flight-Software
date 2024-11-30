/**
 * @file flight_mode.cpp
 * @author csg83, gb486
 *
 * @brief Flight mode logic and transitions
 */

#include "flight_mode.hpp"
#include "constants.hpp"
#include "modules.hpp"
#include "pins.hpp"
#include "state.hpp"

void FlightMode::execute() {
    // Execute the primary functionality of every module
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

    // Clear this cycle's events
    if (!state::flight::events.empty()) {
        state::flight::events.clear();
    }
}

void FlightMode::to_mode(FlightMode *mode) {
    state::flight::mode = mode;
    if (state::fram::init) {
        fram.store(Data::old_mode);
    }
}

// Startup Mode

void StartupMode::execute() {
    // Check to see if the arming key has been turned
    if (gpio_get(ARM_IN)) {
        state::flight::key_armed = true;
        gpio_put(ARM_OUT, 1);
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
    // if (!state::rfm::attempted_init) {
    //     rfm.begin();
    // }
    if (!state::fram::init) {
        fram.begin();
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
    if (state::flight::old_mode == 5) {
        // Transition to Fault Mode if we faulted in the last boot
        to_mode(state::flight::fault);
    } else if (state::flight::key_armed) {
        if (state::alt::status != VALID) {
            // Transition to Fault Mode if the altimeter is non-operational
            to_mode(state::flight::fault);
        } else {
            // Transition to Standby Mode otherwise
            to_mode(state::flight::standby);
        }
    }
}

// Standby Mode

void StandbyMode::execute() {
    FlightMode::execute();

    // Check to see if the arming key has been turned off
    if (!gpio_get(ARM_IN)) {
        state::flight::key_armed = false;
        gpio_put(ARM_OUT, 0);
    }

    // Check the umbilical connection
    // if (!stdio_usb_connected()) {
    //     state::flight::usb_failed_reads++;
    // }

    // Check for any incoming commands
    int c = getchar_timeout_us(0);

    if (c != PICO_ERROR_TIMEOUT) {
        switch ((char)c) {
        case static_cast<char>(Command::launch):
            mav.open(constants::mav_open_time);
            printf("LAUNCH LAUNCH LAUNCH\n\n\n");
            gpio_put(LED, 0);
            state::flight::launch_commanded = true;
            printf("before emplace back\n");
            state::flight::events.emplace_back(Event::launch_command_received);
            break;
        case static_cast<char>(Command::mav_open):
            mav.open();
            printf("MAV OPEN MAV OPEN MAV OPEN\n\n\n");
            state::flight::events.emplace_back(Event::mav_command_received);
            break;
        case static_cast<char>(Command::mav_close):
            mav.close();
            printf("MAV CLOSE MAV CLOSE MAV CLOSE\n\n\n");
            state::flight::events.emplace_back(Event::mav_command_received);
            break;
        case static_cast<char>(Command::sv_open):
            sv.open();
            printf("SV OPEN SV OPEN SV OPEN\n\n\n");
            state::flight::events.emplace_back(Event::sv_command_received);
            break;
        case static_cast<char>(Command::sv_close):
            sv.close();
            printf("SV CLOSE SV CLOSE SV CLOSE\n\n\n");
            state::flight::events.emplace_back(Event::sv_command_received);
            break;
        case static_cast<char>(Command::clear_card):
            sd.clear_card();
            state::flight::events.emplace_back(Event::clear_card_command_received);
            break;
        default:
            printf("COMMAND RECEIVED: %c\n\n\n", (char)c);
            state::flight::events.emplace_back(Event::unknown_command_received);
        }
    }
#ifdef LAUNCH
    // umb.transmit();
#endif
}

void StandbyMode::transition() {
    // Transition to Ascent Mode if launch was commanded through the umbilical
    if (state::flight::launch_commanded) {
        to_mode(state::flight::ascent);
    }
    // Transition to Fault Mode if the umbilical is disconnected
    else if (state::flight::usb_failed_reads == constants::max_usb_failed_reads) {
        sv.open();
        to_mode(state::flight::fault);
    }
    // Transition to Startup Mode if the arming key was turned off
    else if (!state::flight::key_armed) {
        to_mode(state::flight::startup);
    }
}

// Ascent Mode

void AscentMode::execute() {
    printf("in ascent\n");
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
            ssa.trigger(Chute::drogue);
            to_mode(state::flight::drogue_deployed);
        }
    }
}

// Drogue Deployed Mode

void DrogueDeployedMode::transition() {
    // Proceed to Main Deployed mode if the deployment altitude is reached and we have waited for main_deploy_wait
    if (main_cycle_count < constants::main_deploy_wait) {
        main_cycle_count++;
    } else if (main_cycle_count == constants::main_deploy_wait) {
        state::flight::events.emplace_back(Event::main_deploy_wait_end);
        main_cycle_count++;
    } else if (state::alt::altitude < constants::main_deploy_altitude) {
        ssa.trigger(Chute::main);
        state::flight::hold_start = to_ms_since_boot(get_absolute_time());
        state::blims::curr_action_duration = blims.action_arr[state::blims::curr_action_index].duration;
        to_mode(state::flight::main_deployed);
    }
}

// Main Deployed Mode

void MainDeployedMode::execute() {
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
    blims.execute();
}