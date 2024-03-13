/**
 * @file ssa.cpp
 * @brief Unit test of SSA actuation for both main and drogue deployment.
 * @author csg83
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

    uint32_t ematch_start = 0;

    // Mimic the behavior of FSW - Loop continuously while checking the on-time
    printf("Drogue ematch set HIGH\n");
    gpio_put(SSA_1, 1);
    ematch_start = to_ms_since_boot(get_absolute_time());
    while (true) {
        if (to_ms_since_boot(get_absolute_time()) - ematch_start >= constants::ematch_threshold) {
            gpio_put(SSA_1, 0);
            break;
        }
    }
    printf("Drogue ematch set LOW\n");
    sleep_ms(5000);

    printf("Main ematch set HIGH\n");
    gpio_put(SSA_2, 1);
    ematch_start = to_ms_since_boot(get_absolute_time());
    while (true) {
        if (to_ms_since_boot(get_absolute_time()) - ematch_start >= constants::ematch_threshold) {
            gpio_put(SSA_2, 0);
            break;
        }
    }
    printf("Main ematch set LOW\n");

    return 0;
}