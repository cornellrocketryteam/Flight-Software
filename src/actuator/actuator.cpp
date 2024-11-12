/**
 * @file actuator.cpp
 * @author csg83
 *
 * @brief Actuator-related functionality
 */

#include "actuator.hpp"
#include "state.hpp"

MAV::MAV() {}

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

SV::SV() {}

void SV::open() {
}

void SV::close() {
}