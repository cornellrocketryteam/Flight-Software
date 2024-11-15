/**
 * @file flight_mode.cpp
 * @author csg83, gb486
 *
 * @brief Flight mode logic and transitions
 */

#include "flight_mode.hpp"
#include "constants.hpp"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pins.hpp"
#include "state.hpp"

// want wrap to be as large as possible, increases the amount of steps so that we have as much control as possible
uint16_t wrap_cycle_count = 65535; // setting to max value for uint16

float action_arr[10][2] = {
    {0.6, constants::turn_hold_threshold},
    {0.5, constants::neutral_hold_threshold},
    {0.3, constants::turn_hold_threshold},
    {0.5, constants::neutral_hold_threshold},
    {0.8, constants::turn_hold_threshold},
    {0.5, constants::neutral_hold_threshold},
    {0.1, constants::turn_hold_threshold},
    {0.5, constants::neutral_hold_threshold},
    {1.0, constants::turn_hold_threshold},
    {0.5, constants::neutral_hold_threshold},
};
// maintains the action duration across iterations
uint32_t action_duration = 0;
// increases each iteration
int action_index = 0;
// gets beginning time for each motor step
float action_begin_time = 0;
// initial hold
bool run_init_hold = true;
// reached braking altitude
bool brake_alt = false;
////////////

/*
setup_pwm_50hz configures the pwm signal
takes in gpio_pin
*/
void set_motor_position(uint gpio_pin, float position) {
    // Position should be between 0-1
    // Should map between -17 to 17 turns (configured in web UI)
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // Map position to PWM duty cycle (typically 1ms to 2ms pulse width)
    uint16_t five_percent_duty_cycle = wrap_cycle_count * 0.05;
    // ranges between 5% and 10% duty cycle; 3276 ~= 5% duty, 6552 ~= 10% duty
    uint16_t duty = (uint16_t)(five_percent_duty_cycle + position * five_percent_duty_cycle);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio_pin), duty);
}

/*
setup_pwm_50hz configures the pwm signal
takes in gpio_pin
*/
void setup_pwm_50hz(uint gpio_pin) {
    // Set up the PWM configuration
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // What our pico runs - set by the pico hardware itself
    uint32_t clock = 125000000;
    // desired pwm frequency Hz - what we want to achieve
    uint32_t pwm_freq = 50;
    // div is important to get the frequency we want as we lose float info here
    uint32_t divider_int = clock / pwm_freq / wrap_cycle_count;
    // gives us the fractional component of the divider
    uint32_t divider_frac = (clock % (pwm_freq * wrap_cycle_count)) * 16 / (pwm_freq * wrap_cycle_count);
    // Clock divider: slows down pwm to get a certain amount of Hz
    // slice num - depends on what pin we're at, we set pin, pin has set slice num
    // integer divider - going to be 38
    pwm_set_clkdiv_int_frac(slice_num, divider_int, divider_frac);
    pwm_set_wrap(slice_num, wrap_cycle_count);
    pwm_set_enabled(slice_num, true);
}

// SimData sim_data;

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
        fram.store(Data::flight_mode);
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
    // if (state::alt::status == OFF) {
    //     altimeter.begin();
    // }
    // if (state::gps::status == OFF) {
    // }
    // if (state::imu::status == OFF) {
    //     imu.begin();
    // }
    // if (state::accel::status == OFF) {
    //     accel.begin();
    // }
    // if (state::therm::status == OFF) {
    //     therm.begin();
    // }
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
    // if (state::flight::old_mode == 5) {
    //     // Transition to Fault Mode if we faulted in the last boot
    //     to_mode(state::flight::fault);
    // } else if (state::flight::key_armed) {
    //     if (state::alt::status != VALID) {
    //         // Transition to Fault Mode if the altimeter is non-operational
    //         to_mode(state::flight::fault);
    //     } else {
    //         // Transition to Standby Mode otherwise
    //         to_mode(state::flight::standby);
    //     }
    // }
    to_mode(state::flight::standby);
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
    if (!stdio_usb_connected()) {
        state::flight::usb_failed_reads++;
    }

    // Check for any incoming commands
    int c = getchar_timeout_us(0);

    if (c != PICO_ERROR_TIMEOUT) {
        switch ((char)c) {
        case static_cast<char>(Command::launch):
            mav.open(constants::mav_open_time);
            state::flight::launch_commanded = true;
            state::flight::events.emplace_back(Event::launch_command_received);
            break;
        case static_cast<char>(Command::mav_open):
            mav.open();
            state::flight::events.emplace_back(Event::mav_command_received);
            break;
        case static_cast<char>(Command::mav_close):
            mav.close();
            state::flight::events.emplace_back(Event::mav_command_received);
            break;
        case static_cast<char>(Command::sv_open):
            sv.open();
            state::flight::events.emplace_back(Event::sv_command_received);
            break;
        case static_cast<char>(Command::sv_close):
            sv.close();
            state::flight::events.emplace_back(Event::sv_command_received);
            break;
        case static_cast<char>(Command::clear_card):
            sd.clear_card();
            state::flight::events.emplace_back(Event::clear_card_command_received);
            break;
        default:
            state::flight::events.emplace_back(Event::unknown_command_received);
        }
    }

    umb.transmit();
}

void StandbyMode::transition() {
    // Transition to Ascent Mode if launch was commanded through the umbilical
    // if (state::flight::launch_commanded) {
    //     to_mode(state::flight::ascent);
    // }
    // // Transition to Fault Mode if the umbilical is disconnected
    // else if (state::flight::usb_failed_reads == constants::max_usb_failed_reads) {
    //     sv.open();
    //     to_mode(state::flight::fault);
    // }
    // // Transition to Startup Mode if the arming key was turned off
    // else if (!state::flight::key_armed) {
    //     to_mode(state::flight::startup);
    // }
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
    // if (state::alt::status == VALID && state::flight::alt_armed) {
    //     // If we're armed and the altimeter is valid, start checking for apogee
    //     alt_sum -= alt_buffer[index];
    //     alt_buffer[index] = state::alt::altitude;
    //     alt_sum += state::alt::altitude;
    //     index++;
    //     if (index == 10) {
    //         index = 0;
    //     }

    //     if (count == interval) {
    //         filtered_alt[2] = filtered_alt[1];
    //         filtered_alt[1] = filtered_alt[0];
    //         filtered_alt[0] = alt_sum * 0.1;
    //         count = 0;
    //     }
    //     count++;

    //     if (filtered_alt[2] != -1 && filtered_alt[1] != -1 && filtered_alt[0] != -1 &&
    //         filtered_alt[2] > filtered_alt[1] && filtered_alt[1] > filtered_alt[0]) {
    //         gpio_put(SSA_DROGUE, 1);
    //         state::flight::ematch_start = to_ms_since_boot(get_absolute_time());
    //         to_mode(state::flight::drogue_deployed);
    //     }
    // }
    to_mode(state::flight::drogue_deployed);
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
    // if (main_cycle_count < constants::main_deploy_wait) {
    //     main_cycle_count++;
    // } else if (main_cycle_count == constants::main_deploy_wait) {
    //     state::flight::events.emplace_back(Event::main_deploy_wait_end);
    //     main_cycle_count++;
    // } else if (state::alt::altitude < constants::main_deploy_altitude) {
    //     gpio_put(SSA_MAIN, 1);
    //     state::flight::ematch_start = to_ms_since_boot(get_absolute_time());
    //     ////////
    state::flight::hold_start = to_ms_since_boot(get_absolute_time());
    action_duration = action_arr[action_index][1];
    //     ////////
    //     to_mode(state::flight::main_deployed);
    // }
    to_mode(state::flight::main_deployed);
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

    FlightMode::execute(); // updates state of rocket with curr sensor readings, every 50ms
    // assume motor starts as neutral
    logf("TEST 1\n");
    if (to_ms_since_boot(get_absolute_time()) - state::flight::hold_start >= constants::initial_hold_threshold) {
        run_init_hold = false;
        logf("TEST 2\n");
    }
    uint32_t curr_time = to_ms_since_boot(get_absolute_time());
    logf("Current Time: %d\n", curr_time);
    // time at beginning of cycle - curr_time
    action_duration -= (state::flight::timestamp - curr_time);
    // action_duration -= curr_time;
    logf("Action Duration: %d\n", action_duration);

    // // check time
    // setup_pwm_50hz(BLIMS_MOTOR);
    // ///
    // set_motor_position(BLIMS_MOTOR, 0.0);
    // sleep_ms(5000);
    // logf("Motor 1 Done");

    // set_motor_position(BLIMS_MOTOR, 0.5);
    // sleep_ms(5000);
    // logf("Motor 2 Done");

    // set_motor_position(BLIMS_MOTOR, 1.0);
    // sleep_ms(5000);
    // logf("Motor 3 Done");
    // if (action_duration < 0 && !run_init_hold && !brake_alt)
    if (action_duration < 0) {
        logf("TEST 3\n");
        action_index = action_index + 1;
        action_duration = action_arr[action_index][1];
        set_motor_position(BLIMS_MOTOR, action_arr[action_index][0]);
        logf("TEST 4\n");
    }
    logf("TEST 5\n");
    // check altitude
    // if (state::alt::altitude < constants::brake_alt) {
    //     set_motor_position(BLIMS_MOTOR, constants::neutral_pos);
    //     brake_alt = true;
    // }
}