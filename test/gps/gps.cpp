/**
 * @file gps.cpp
 * @author csg83
 * 
 * @brief Unit test of GPS data receiving
 */

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "tusb.h"
#include "../../src/pins.hpp"
#include "../../src/constants.hpp"

#define UBLOX_ADDR 0x42

void read_gps_data() {
    uint8_t buffer[256];
    int bytes_read = i2c_read_blocking(I2C_PORT, UBLOX_ADDR, buffer, sizeof(buffer), false);
    if (bytes_read > 0) {
        printf("%.*s\n", bytes_read, buffer);
    } else {
        printf("Read failed\n");
    }
}

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    while (true) {
        read_gps_data();
    }

    return 0;
}