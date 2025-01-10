/**
 * @file actuate_sv.cpp
 * @author csg83
 * 
 * @brief Unit test of SV actuation
 */

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "../../src/pins.hpp"
#include "hardware/timer.h"
#include "tusb.h"
#include <cstdio>

static int64_t hold_pwm(alarm_id_t id, void *user_data) {
    printf("Setting PWM\n");
    pwm_set_gpio_level(RELAY, 1707);
    return 0;
}

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    gpio_set_function(RELAY, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(RELAY);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 2);
    pwm_config_set_wrap(&config, 4095);
    pwm_init(slice_num, &config, true);

    printf("Setting SV off\n");
    pwm_set_gpio_level(RELAY, 0);

    sleep_ms(3000);

    printf("Setting SV on\n");
    pwm_set_gpio_level(RELAY, 4095);
    add_alarm_in_ms(150, hold_pwm, NULL, true);

   
    sleep_ms(5000);

    printf("Setting SV off\n");
    pwm_set_gpio_level(RELAY, 0);

    return 0;
}
