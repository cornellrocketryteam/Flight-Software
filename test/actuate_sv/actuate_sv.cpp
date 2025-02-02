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
#ifdef VERBOSE
#include "tusb.h"
#endif
#include <cstdio>

static int64_t hold_pwm(alarm_id_t id, void *user_data) {
#ifdef VERBOSE
    printf("Setting PWM\n");
#endif
    pwm_set_gpio_level(RELAY, 1707);
    return 0;
}

int main() {
    stdio_init_all();

#ifdef VERBOSE
    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");
#endif

    gpio_set_function(RELAY, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(RELAY);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 2);
    pwm_config_set_wrap(&config, 4095);
    pwm_init(slice_num, &config, true);

#ifdef VERBOSE
    printf("Setting SV off\n");
#endif
    pwm_set_gpio_level(RELAY, 0);

    sleep_ms(3000);

#ifdef VERBOSE
    printf("Setting SV on\n");
#endif
    pwm_set_gpio_level(RELAY, 4095);
    add_alarm_in_ms(150, hold_pwm, NULL, true);

   
    sleep_ms(5000);

#ifdef VERBOSE
    printf("Setting SV off\n");
#endif
    pwm_set_gpio_level(RELAY, 0);

    return 0;
}
