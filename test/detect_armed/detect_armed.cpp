/**
 * @file detect_armed.cpp
 * @author csg83
 * 
 * @brief Unit test for detecting the arming key
 */

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tusb.h"
#include "../../src/pins.hpp"
#include "../../src/constants.hpp"

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    gpio_init(ARM_IN);
    gpio_set_dir(ARM_IN, GPIO_IN);

    bool key_armed = false;

    while (true) {
        if (key_armed != gpio_get(ARM_IN)) {
            key_armed = !key_armed;
            if (key_armed) {
                printf("Armed from GPIO %u\n", ARM_IN);
            } else {
                printf("Disarmed from GPIO %u\n", ARM_IN);
            }
            gpio_put(ARM_OUT, key_armed);
        }

        sleep_ms(constants::cycle_time);
    }

    return 0;
}