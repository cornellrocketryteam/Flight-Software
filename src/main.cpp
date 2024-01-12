#include <cstdio>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#ifdef DEBUG
#include "tusb.h"
#endif

#include "core/fsw.hpp"
#include "pins.hpp"

Flight flight;

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    gpio_init(ARMED);
    gpio_set_dir(ARMED, GPIO_IN);

    gpio_init(SSA_1);
    gpio_set_dir(SSA_1, GPIO_OUT);

    gpio_init(SSA_2);
    gpio_set_dir(SSA_2, GPIO_OUT);

    gpio_init(RFM_CS);
    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_put(RFM_CS, 1);

#ifdef DEBUG
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
