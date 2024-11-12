/**
 * @file actuator.hpp
 * @author csg83
 *
 * @brief Actuator-related definitions
 */

#ifndef ACTUATOR_HPP
#define ACTUATOR_HPP

#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pins.hpp"

class MAV {
public:
    MAV();
    void open(uint duration = 0);
    void close();

private:
    void set_position(float position);
    uint slice_num = pwm_gpio_to_slice_num(MAV_SIGNAL);
};

class SV {
public:
    SV();
    void open();
    void close();
};

#endif // ACTUATOR_HPP