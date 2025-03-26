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

enum class Chute : uint8_t {
    drogue = SSA_DROGUE,
    main = SSA_MAIN
};

class SSA {
public:
    void trigger(Chute chute);

private:
    static int64_t time_off(alarm_id_t id, void *user_data);
};

class Buzzer {
public:
    void on();
    void off();

    void buzz_num(uint buzzes);

private:
    static inline uint buzzer_slice_num = pwm_gpio_to_slice_num(ARM_OUT);
    static bool buzz(struct repeating_timer *t);
    struct repeating_timer timer;
};

class MAV {
public:
    void open(uint duration = 0);
    void close();

private:
    static inline uint mav_slice_num = pwm_gpio_to_slice_num(MAV_SIGNAL);
    void set_position(float position);
    static int64_t time_close(alarm_id_t id, void *user_data);
};

class SV {
public:
    void open();
    void close();

private:
    static inline uint sv_slice_num = pwm_gpio_to_slice_num(SV_SIGNAL);
    static int64_t hold_pwm(alarm_id_t id, void *user_data);
};

#endif // ACTUATOR_HPP