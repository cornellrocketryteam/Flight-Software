#include <cstdio>

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#ifdef DEBUG
#include "tusb.h"
#endif

#include "core/fsw.hpp"
#include "pins.hpp"

Flight flight;

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    gpio_init(ARMED);
    gpio_set_dir(ARMED, GPIO_IN);

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
