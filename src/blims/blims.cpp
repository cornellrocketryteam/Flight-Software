/**
 * @file blims.cpp
 * @author gb486
 *
 * @brief BLIMS Functionality
 */

#include "blims.hpp"
#include "../core/state.hpp"
#include "hardware/pwm.h"

void BLIMS::set_motor_position(float position) {
    // Position should be between 0-1
    // Should map between -17 to 17 turns (configured in web UI)

    // Map position to PWM duty cycle (typically 1ms to 2ms pulse width)
    uint16_t five_percent_duty_cycle = wrap_cycle_count * 0.05;
    // ranges between 5% and 10% duty cycle; 3276 ~= 5% duty, 6552 ~= 10% duty
    uint16_t duty = (uint16_t)(five_percent_duty_cycle + position * five_percent_duty_cycle);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(BLIMS_MOTOR), duty);

    // update state of motor (what is the position at the current time)
    state::blims::motor_position = position;
}

void BLIMS::pwm_setup() {
      // Set up the PWM configuration
    gpio_set_function(BLIMS_MOTOR, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BLIMS_MOTOR);

    uint32_t clock = 125000000; // What our pico runs - set by the pico hardware itself

    uint32_t pwm_freq = 50; // desired pwm frequency Hz - what we want to achieve

    uint32_t divider_int = clock / pwm_freq / wrap_cycle_count; // div is important to get the frequency we want as we lose float info here

    uint32_t divider_frac = (clock % (pwm_freq * wrap_cycle_count)) * 16 / (pwm_freq * wrap_cycle_count); // gives us the fractional component of the divider
      // Clock divider: slows down pwm to get a certain amount of Hz
      // slice num - depends on what pin we're at, we set pin, pin has set slice num
      // integer divider - going to be 38
    pwm_set_clkdiv_int_frac(slice_num, divider_int, divider_frac);
    pwm_set_wrap(slice_num, wrap_cycle_count);
    pwm_set_enabled(slice_num, true);
}
void BLIMS::execute() {
    // don't want to do anything if brake_alt is true
    if (state::blims::below_brake_alt) {
        return;
    }

    // assume motor starts as neutral
    if (run_init_hold && (to_ms_since_boot(get_absolute_time()) - state::flight::hold_start >= constants::initial_hold_threshold)) {
        run_init_hold = false;
        printf("TEST 2\n\n\n\n\n");
    }
    // don't want to increase curr_action duration during initial hold
    if (run_init_hold) {
        return;
    }

    state::blims::curr_action_duration -= (state::flight::timestamp - curr_time);
    curr_time = to_ms_since_boot(get_absolute_time());

    if (state::blims::curr_action_duration <= 0 && !run_init_hold) {
        if (state::blims::curr_action_index >= 10) { // length calculation - need to fix and get size of action_arr
            state::blims::curr_action_index = 0;
            printf("TEST 4\n");
        }
        printf("Action Duration: %d\n", state::blims::curr_action_duration);
        printf("Action Index: %d\n", state::blims::curr_action_index);
        state::blims::curr_action_duration = action_arr[state::blims::curr_action_index].duration;
        set_motor_position(action_arr[state::blims::curr_action_index].position);
        state::blims::curr_action_index = state::blims::curr_action_index + 1;
        state::flight::events.emplace_back(Event::blims_threshold_reached); // we've completed a motor action in action_arr
    }
    printf("TEST 5\n");
    // check altitude
    // if (state::alt::altitude < constants::brake_alt) {
    //     set_motor_position(0.5);
    //     state::blims::below_brake_alt = true;
    //     state::flight::events.emplace_back(Event::below_brake_alt); // logs the event to SD
    // }
}
