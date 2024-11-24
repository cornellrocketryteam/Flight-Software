/**
 * @file actuator.cpp
 * @author csg83
 *
 * @brief Actuator-related functionality
 */

#include "actuator.hpp"
#include "constants.hpp"
#include "hardware/timer.h"
#include "state.hpp"

// SSA

void SSA::trigger(Chute chute) {
    gpio_put(static_cast<uint8_t>(chute), 1);
    add_alarm_in_ms(constants::ematch_threshold, callback, reinterpret_cast<void *>(static_cast<uintptr_t>(chute)), true);
}

int64_t SSA::callback(alarm_id_t id, void *user_data) {
    Chute chute = static_cast<Chute>(reinterpret_cast<uintptr_t>(user_data));
    gpio_put(static_cast<uint8_t>(chute), 0);
    return 0;
}

// MAV

void MAV::open(uint duration) {
    state::mav::open = true;
    set_position(0.5);

    if (duration) {
        // TODO: Set timer
    }
}

void MAV::close() {
    state::mav::open = false;
    set_position(0);
}

void MAV::set_position(float position) {
    uint16_t dc = (uint16_t)(21845 + position * 21845);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(MAV_SIGNAL), dc);
}

// SV

void SV::open() {
}

void SV::close() {
}