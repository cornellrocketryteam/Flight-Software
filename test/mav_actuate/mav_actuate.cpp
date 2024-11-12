/**
 * @file mav_actuate.cpp
 * @author csg83, zag7
 * 
 * @brief Unit test of MAV actuation
 */

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "../../src/pins.hpp"

void set_position(float position) {
    uint slice_num = pwm_gpio_to_slice_num(MAV);
    uint16_t dc = (uint16_t)(21845 + position * 21845);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(MAV), dc);
}

int main() {
    stdio_init_all();

    gpio_set_function(MAV, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(MAV);
    
    uint8_t divider = 125000000 / 330 / 65535;
    pwm_set_clkdiv_int_frac(slice_num, divider, 12);
    pwm_set_wrap(slice_num, 65535);
    pwm_set_enabled(slice_num, true);

    while (true) {
        set_position(0.0);
        sleep_ms(5000);

        set_position(0.5);
        sleep_ms(5000);

        set_position(1.0);
        sleep_ms(5000);
    }

    return 0;
}
