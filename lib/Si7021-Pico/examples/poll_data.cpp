#include "../si7021.hpp"
#include "tusb.h"
#include <cstdio>

#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5

Si7021 therm(I2C_PORT);

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    if (therm.begin()) {
        printf("Init successful\n");
    } else {
        printf("Init failed\n");
        return 1;
    }
    float temp, humidity;

    while (true) {
        temp = therm.read_temperature();
        humidity = therm.read_humidity();
        printf("Temperature: %.3f\n", temp);
        printf("Humidity: %.3f\n", humidity);
        sleep_ms(200);
    }

    return 0;
}