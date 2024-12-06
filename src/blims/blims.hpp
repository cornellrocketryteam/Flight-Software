/**
 * @file blims.hpp
 * @author gb486
 *
 * @brief BLiMS related definitions
 */
#ifndef BLIMS_HPP
#define BLIMS_HPP
#include "../constants.hpp"
#include "../pins.hpp"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

struct Action {
    float position;
    uint32_t duration;
};

class BLIMS {
public:
    // void execute();
    constexpr static Action action_arr[10] = {
        {0.6f, constants::turn_hold_threshold},
        {0.5f, constants::neutral_hold_threshold},
        {0.3f, constants::turn_hold_threshold},
        {0.5f, constants::neutral_hold_threshold},
        {0.8f, constants::turn_hold_threshold},
        {0.5f, constants::neutral_hold_threshold},
        {0.1f, constants::turn_hold_threshold},
        {0.5f, constants::neutral_hold_threshold},
        {1.0f, constants::turn_hold_threshold},
        {0.5f, constants::neutral_hold_threshold},
    };
    void pwm_setup();

    static int64_t execute(alarm_id_t id, void *user_data);

private:
  // sets position of motor on a 0-1 scale
    static void set_motor_position(float position);

  // pwm_setup configures the pwm signal

      // want wrap to be as large as possible, increases the amount of steps so that we have as much control as possible
    uint16_t wrap_cycle_count = 65535;
    uint32_t curr_time = 0;
    uint slice_num = pwm_gpio_to_slice_num(BLIMS_MOTOR);

  // initial hold
    bool run_init_hold = true;
  // reached braking altitude
};

#endif