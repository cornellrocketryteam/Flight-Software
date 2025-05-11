/**
 * @file flight_mode.cpp
 * @author csg83, gb486
 *
 * @brief Flight mode logic and transitions
 */

#include "flight_mode.hpp"
#include "constants.hpp"
#include "events.hpp"
#include "modules.hpp"
#include "pins.hpp"
#include "state.hpp"

void FlightMode::execute() {
    // Execute the primary functionality of every module
    if (state::alt::status != OFF) {
        altimeter.read_altitude();
    }
    if (state::gps::status != OFF) {
        gps.read_data();
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
    if (state::adc::status != OFF) {
        adc.read_data();
    }
    if (state::sd::init) {
        sd.log();
    }
    adc.read_internal();
    rfm.transmit();

    // Clear this cycle's events
    if (!events.is_empty()) {
        events.clear();
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
    }

    // Attempt to initialize all modules
    if (state::alt::status == OFF) {
        altimeter.begin();
    }
    if (state::gps::status == OFF) {
        gps.begin();
    }
    if (state::imu::status == OFF) {
        imu.begin();
    }
    if (state::accel::status == OFF) {
        accel.begin();
    }
    if (state::adc::status == OFF) {
        adc.begin();
    }
    if (!state::fram::init) {
        fram.begin();
    }
    if (!state::sd::init) {
        sd.begin();
    }
    rfm.transmit();

    blims_obj.begin(state::blims::blims_mode, BLIMS_SIGNAL, BLIMS_ENABLE, state::blims::target_lat, state::blims::target_long);

    // Check the umbilical connection
    if (umb.connection_changed()) {
        if (state::umb::connected) {
            buzzer.buzz_num(2);
        } else {
            buzzer.buzz_num(3);
        }
    }

    // Continuously update reference pressure before launch
    if (state::alt::status == VALID) {
        altimeter.update_ref_pressure();
    }

    umb.check_command();
#ifdef LAUNCH
    umb.transmit();
#endif
}

void StartupMode::transition() {
    if (state::flight::old_mode == 5) {
        // Transition to Fault if we faulted in the last boot
        to_mode(state::flight::fault);
    } else if (state::flight::key_armed) {
        if (state::alt::status != VALID) {
            // Transition to Fault if the altimeter is non-operational
            // to_mode(state::flight::fault);
            to_mode(state::flight::standby); // TODO Change
        } else {
            // Transition to Standby otherwise
            buzzer.buzz_num(1);
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
    }

    // Check the umbilical connection
    if (umb.connection_changed()) {
        if (state::umb::connected) {
            buzzer.buzz_num(2);
        } else {
            buzzer.buzz_num(3);
        }
    }

    umb.check_command();
#ifdef LAUNCH
    umb.transmit();
#endif
}

void StandbyMode::transition() {
    // Transition to Ascent if launch was commanded through the umbilical
    if (state::umb::launch_commanded) {
        mav.open(constants::mav_open_time);
        sv.open(constants::sv_open_delay_time);
        altimeter.update_ref_pressure(true);
        to_mode(state::flight::ascent);
    }
    // Transition to Startup if the arming key was turned off
    else if (!state::flight::key_armed) {
        buzzer.buzz_num(1);
        to_mode(state::flight::startup);
    }
#ifdef LAUNCH
    // Transition to Fault if the umbilical is disconnected
    else if (!state::umb::connected) {
        sv.open();
        events.push(Event::umbilical_disconnected);
        // to_mode(state::flight::fault); // TODO add back in
    }
#endif
}

// Ascent Mode

void AscentMode::execute() {
    FlightMode::execute();

    // Check for arming altitude
    if (state::alt::status == VALID && !state::flight::alt_armed && state::alt::altitude > constants::arming_altitude) {
        state::flight::alt_armed = true;
        events.push(Event::alt_armed);
    }

    // Write PT data to FRAM upon SD failure
    if (!state::sd::init) {
        fram.store(Data::pt);
    }

    umb.check_command();
#ifdef LAUNCH
    umb.transmit();
#endif
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

        if (count == constants::alt_sample_interval) {
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
    // Proceed to Main Deployed if the deployment altitude is reached and we have waited for main_deploy_wait
    if (main_cycle_count < constants::main_deploy_wait) {
        main_cycle_count++;
    } else if (main_cycle_count == constants::main_deploy_wait) {
        events.push(Event::main_deploy_wait_end);
        main_cycle_count++;
    } else if (state::alt::altitude < constants::main_deploy_altitude) {
        ssa.trigger(Chute::main);
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
        events.push(Event::main_log_shutoff);
        log_cycle_count++;
    }

    FlightMode::execute();

    MainDeployedMode::to_blims_data = {
                                        // in future: let's send gps struct
        .lon = state::gps::data.lon,
        .lat = state::gps::data.lat,
        .hAcc = state::gps::data.hAcc,
        .vAcc = state::gps::data.vAcc,
        .velN = state::gps::data.velN,
        .velE = state::gps::data.velE,
        .velD = state::gps::data.velD,
        .gSpeed = state::gps::data.gSpeed,
        .headMot = state::gps::data.headMot,
        .sAcc = state::gps::data.sAcc,
        .headAcc = state::gps::data.headAcc

    };
    MainDeployedMode::from_blims_data = blims_obj.execute(&to_blims_data);
    // pointers -> keeps data same size
    // takes two params = *to_blimsdata, *from_blims_data
    // function returns void
    // alter motor position by using address passeed in within blims module

    // save motor position in fsw
    state::blims::motor_position = MainDeployedMode::from_blims_data.motor_position;
}

// Fault Mode

void FaultMode::execute() {
    FlightMode::execute();

    umb.check_command();
#ifdef LAUNCH
    umb.transmit();
#endif
}