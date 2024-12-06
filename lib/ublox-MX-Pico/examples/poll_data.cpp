#include "../ublox_mx.hpp"
#include "tusb.h"

#define I2C_PORT i2c0
#define I2C_SDA 12
#define I2C_SCL 13

GNSS gps(I2C_PORT);

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

    if (gps.begin()) {
        printf("Init successful\n");
    } else {
        printf("Init failed\n");
        return 1;
    }

    // gps.disable_message();
    sleep_ms(100);
    // gps.send_config_msg(0);

    gnss_data_t data;

    while (true) {
        if (gps.read_data(&data)) {
            printf("Latitude: %s\n", data.latitude.c_str());
            printf("Longitude: %s\n", data.longitude.c_str());
            printf("Num Satellites: %s\n", data.num_satellites.c_str());
        }
        sleep_ms(1000);
    }
}