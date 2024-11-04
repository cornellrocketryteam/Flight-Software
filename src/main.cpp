#include <cstdio>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#ifndef LAUNCH
#include "tusb.h"
#endif

#include "fsw.hpp"
#include "pins.hpp"

Flight flight;

void init_pins() {
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 1);

    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    gpio_init(ARM_IN);
    gpio_set_dir(ARM_IN, GPIO_IN);

    gpio_init(ARM_OUT);
    gpio_set_dir(ARM_OUT, GPIO_OUT);

    gpio_init(SSA_DROGUE);
    gpio_set_dir(SSA_DROGUE, GPIO_OUT);

    gpio_init(SSA_MAIN);
    gpio_set_dir(SSA_MAIN, GPIO_OUT);

    gpio_init(RFM_CS);
    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_put(RFM_CS, 1);

    gpio_init(RFM_RST);
    gpio_set_dir(RFM_RST, GPIO_OUT);
}

int main() {
    stdio_init_all();
#ifdef LAUNCH
    sleep_ms(3000);
    printf("Unplug from computer\n");
    sleep_ms(5000);
    printf("WARNING: Boot file has been modified\n");
    sleep_ms(1000);
#endif

    init_pins();

#ifndef LAUNCH
    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");
#endif

    while (true) {
        flight.execute();
    }
    return 0;
}
