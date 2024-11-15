

#include "pico/stdlib.h"
#include <cstdio>
#include "tusb.h"
#include "../../src/pins.hpp"
#include "../../src/constants.hpp"

#include "bmp388.hpp"
#include "bno055.hpp"
#include "lis3dh.hpp"
#include "si7021.hpp"
#include "mb85rs.hpp"

#include <functional>
#include <tuple>

#define NUM_RUNS 1000

BMP388 altimeter(I2C_PORT);
LIS3DH accel(I2C_PORT);
BNO055 imu(I2C_PORT);
Si7021 therm(I2C_PORT);

MB85RS fram(SPI_PORT, FRAM_CS);

uint64_t start, duration;

template <typename Func, typename... Args>
uint64_t time_func(Func func, Args... args) {
    start = time_us_64();
    func(args...);
    duration = time_us_64() - start;
    return duration;
}

void time_begin() {
    
}

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    // Init LED and turn on immediately
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 1);

    // Init GPIO pins
    gpio_init(ARM_IN);
    gpio_init(ARM_OUT);
    gpio_init(SSA_DROGUE);
    gpio_init(SSA_MAIN);
    gpio_init(FRAM_CS);

    gpio_set_dir(ARM_IN, GPIO_IN);
    gpio_set_dir(ARM_OUT, GPIO_OUT);
    gpio_set_dir(SSA_DROGUE, GPIO_OUT);
    gpio_set_dir(SSA_MAIN, GPIO_OUT);
    gpio_set_dir(FRAM_CS, GPIO_OUT);

    // Init I2C bus
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Init SPI bus
    spi_init(SPI_PORT, constants::spi_baudrate);
    gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
    gpio_put(FRAM_CS, 1);

    uint64_t duration;

    printf("========== Begin ==========\n\n");

    duration = time_func([&]() { altimeter.begin(); });
    printf("Altimeter begin(): %llu\n", duration);

    duration = time_func([&]() { accel.begin(); });
    printf("Accel begin(): %llu\n", duration);

    duration = time_func([&]() { imu.begin(); });
    printf("IMU begin(): %llu\n", duration);

    duration = time_func([&]() { therm.begin(); });
    printf("Therm begin(): %llu\n", duration);

    duration = time_func([&]() { fram.begin(); });
    printf("FRAM begin(): %llu\n", duration);

    printf("========== Reads ==========\n\n");
    float val;
    float x, y, z;
    uint64_t sum = 0;

    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { altimeter.read_altitude(&val, 1); });
        sum += duration;
        sleep_ms(5);
    }
    printf("Altimeter read_altitude(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { accel.read_accel(&x, &y, &z); });
        sum += duration;
        sleep_ms(5);
    }
    printf("Accel read_accel(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { imu.read_accel(&x, &y, &z); });
        sum += duration;
        sleep_ms(5);
    }
    printf("IMU read_accel(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { imu.read_gyro(&x, &y, &z); });
        sum += duration;
        sleep_ms(5);
    }
    printf("IMU read_gyro(): %llu\n", sum / NUM_RUNS);

    

    return 0;
}