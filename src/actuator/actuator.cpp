/**
 * @file actuator.cpp
 * @author csg83
 *
 * @brief Actuator-related functionality
 */

#include "actuator.hpp"
#include "constants.hpp"
#include "hardware/timer.h"
#include "modules.hpp"
#include "state.hpp"

// SSA

void SSA::trigger(Chute chute) {
    gpio_put(static_cast<uint8_t>(chute), 1);
    add_alarm_in_ms(constants::ematch_threshold, time_off, reinterpret_cast<void *>(static_cast<uintptr_t>(chute)), true);
}

int64_t SSA::time_off(alarm_id_t id, void *user_data) {
    Chute chute = static_cast<Chute>(reinterpret_cast<uintptr_t>(user_data));
    gpio_put(static_cast<uint8_t>(chute), 0);
    return 0;
}

// Buzzer

// TODO: Look into moving these
bool is_wait = false;
uint num_buzzes = 0;

void Buzzer::on() {
    pwm_set_enabled(buzzer_slice_num, true);
}

void Buzzer::off() {
    pwm_set_enabled(buzzer_slice_num, false);
}

void Buzzer::buzz_num(uint buzzes) {
    if (num_buzzes != 0) {
        return;
    }
    num_buzzes = buzzes;
    add_repeating_timer_ms(constants::buzz_delay, buzz, NULL, &timer);
}

bool Buzzer::buzz(struct repeating_timer *t) {
    if (is_wait) {
        pwm_set_enabled(buzzer_slice_num, false);
        is_wait = false;

        num_buzzes--;
        if (num_buzzes <= 0) {
            cancel_repeating_timer(t);
            return false;
        }
    } else {
        pwm_set_enabled(buzzer_slice_num, true);
        is_wait = true;
    }
    return true;
}

// MAV

void MAV::open(uint duration) {
    set_position(0);

    if (duration) {
        add_alarm_in_ms(duration, time_close, NULL, true);
    }

    state::actuator::mav_open = true;
    fram.store(Data::mav_state);
}

void MAV::close() {
    set_position(1.0);
    state::actuator::mav_open = false;
    fram.store(Data::mav_state);
}

void MAV::set_position(float position) {
    uint16_t dc = (uint16_t)(21845 + position * 21845);
    pwm_set_chan_level(mav_slice_num, pwm_gpio_to_channel(MAV_SIGNAL), dc);
}

int64_t MAV::time_close(alarm_id_t id, void *user_data) {
    state::actuator::mav_open = false;
    pwm_set_chan_level(mav_slice_num, pwm_gpio_to_channel(MAV_SIGNAL), 21845);
    return 0;
}

// SV

void SV::open() {
    gpio_put(SV_SIGNAL, false);
    // pwm_set_gpio_level(SV_SIGNAL, 0);
    state::actuator::sv_open = true;
    fram.store(Data::sv_state);
}

void SV::close() {
    gpio_put(SV_SIGNAL, true);
    // pwm_set_gpio_level(SV_SIGNAL, 4095);
    // add_alarm_in_ms(constants::sv_peak_threshold, hold_pwm, NULL, true);
    state::actuator::sv_open = false;
    fram.store(Data::sv_state);
}

int64_t SV::hold_pwm(alarm_id_t id, void *user_data) {
    pwm_set_gpio_level(SV_SIGNAL, 1707);
    return 0;
}