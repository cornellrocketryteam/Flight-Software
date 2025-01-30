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

class MAV {
public:
    void open(uint duration = 0);
    void close();

private:
    void set_position(float position);
    static int64_t time_close(alarm_id_t id, void *user_data);
};

class SV {
public:
    void open();
    void close();

private:
    static int64_t hold_pwm(alarm_id_t id, void *user_data);
};

#endif // ACTUATOR_HPP