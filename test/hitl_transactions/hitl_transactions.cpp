/**
 * @file hitl_transactions.cpp
 * @author csg83
 * 
 * @brief Unit test of all I2C transactions for the altimeter that HITL software will see
 */

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "tusb.h"
#include "../../src/pins.hpp"
#include "../../sim/hitl.hpp"

HITL alt(I2C_PORT);

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    alt.begin();

    float altitude = 0;

    while (true) {
        alt.read_altitude(&altitude, 0);
        printf("Altitude: %.3f\n", altitude);
        sleep_ms(50);
    }

    return 0;
}