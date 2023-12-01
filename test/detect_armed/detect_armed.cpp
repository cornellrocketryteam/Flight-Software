#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tusb.h"

#define ARMED_PIN (6)

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    gpio_init(ARMED_PIN);
    gpio_set_dir(ARMED_PIN, GPIO_IN);

    while (true) {
        if (gpio_get(ARMED_PIN)) {
            printf("Armed from GPIO %u\n", ARMED_PIN);
            break;
        }
        sleep_ms(100);
    }

    return 0;
}