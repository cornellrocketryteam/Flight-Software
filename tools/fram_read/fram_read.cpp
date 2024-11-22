/**
 * @file fram_read.cpp
 * @author csg83
 * 
 * @brief Reads out the contents of FRAM
 */

#include "pico/stdlib.h"
#include "mb85rs.hpp"
#include "../../src/pins.hpp"
#include "tusb.h"
#include <cstdio>

#define BYTES_TO_READ 20

MB85RS fram(SPI_PORT, FRAM_CS);

int main() {
    stdio_init_all();

    gpio_init(FRAM_CS);
    gpio_set_dir(FRAM_CS, GPIO_OUT);

    spi_init(SPI_PORT, 125 * 1000000 / 6);
    gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
    gpio_put(FRAM_CS, 1);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    if (!fram.begin()) {
        printf("Error: FRAM begin unsuccessful\n");
    }

    uint8_t data[BYTES_TO_READ] = {0};
    uint8_t temp = 42;

    fram.write_bytes(0, &temp, 1);

    if (fram.read_bytes(0, data, BYTES_TO_READ)) {
        for (int i = 0; i < BYTES_TO_READ; i++) {
            printf("(%d): %d\n", i, data[i]);
        }
    } else {
        printf("Error: FRAM read unsuccessful\n");
    }

    return 0;
}