#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tusb.h"
#include "../../src/pins.hpp"

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    gpio_init(ARMED_IN);
    gpio_set_dir(ARMED_IN, GPIO_IN);

    while (true) {
        if (gpio_get(ARMED_IN)) {
            printf("Armed from GPIO %u\n", ARMED_IN);
            gpio_put(ARMED_OUT, 1);
        }
        sleep_ms(100);
    }

    return 0;
}