#include <cstdio>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#ifndef LAUNCH
#include "tusb.h"
#endif

#include "core/fsw.hpp"
#include "pins.hpp"

Flight flight;

int main() {
    stdio_init_all();
#ifdef LAUNCH
    sleep_ms(3000);
    printf("Unplug from computer\n");
    sleep_ms(5000);
    printf("ERROR: Too late. Restart process\n");
    sleep_ms(1000);
#endif

    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    gpio_init(ARMED_IN);
    gpio_set_dir(ARMED_IN, GPIO_IN);

    gpio_init(ARMED_OUT);
    gpio_set_dir(ARMED_OUT, GPIO_OUT);

    gpio_init(SSA_1);
    gpio_set_dir(SSA_1, GPIO_OUT);

    gpio_init(SSA_2);
    gpio_set_dir(SSA_2, GPIO_OUT);

    gpio_init(RFM_CS);
    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_put(RFM_CS, 1);

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
