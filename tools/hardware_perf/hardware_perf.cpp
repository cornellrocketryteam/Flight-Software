

#include "pico/stdlib.h"
#include <cstdio>
#include "tusb.h"
#include "../../src/pins.hpp"
#include "../../src/constants.hpp"
#include "f_util.h"
#include "ff.h"
#include "hw_config.h"

#include "bmp388.hpp"
#include "bno055.hpp"
#include "lis3dh.hpp"
#include "mb85rs.hpp"

#include <functional>
#include <tuple>

#define NUM_RUNS 1

BMP388 altimeter(I2C_PORT);
LIS3DH accel(I2C_PORT);
BNO055 imu(I2C_PORT);

MB85RS fram(SPI_PORT, FRAM_CS);

uint64_t start, duration;

template <typename Func, typename... Args>
uint64_t time_func(Func func, Args... args) {
    start = time_us_64();
    func(args...);
    duration = time_us_64() - start;
    return duration;
}

FATFS fs;
FRESULT fr;
char buffer[205];

void sd_begin() {
    fr = f_mount(&fs, "", 1);
    if (FR_OK != fr) {
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }
}

void sd_write() {
    FIL fil;
    const char* const filename = "test.txt";
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);

    if (FR_OK != fr && FR_EXIST != fr) {
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    }
    
    if (f_printf(&fil, buffer) < 0) {
        printf("f_printf failed\n");
    }

    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
}

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    init_pins();

    uint64_t duration;

    printf("=========== Begin ===========\n\n");

    duration = time_func([&]() { altimeter.begin(); });
    printf("Altimeter begin(): %llu\n", duration);

    duration = time_func([&]() { accel.begin(); });
    printf("Accel begin(): %llu\n", duration);

    duration = time_func([&]() { imu.begin(); });
    printf("IMU begin(): %llu\n\n", duration);

    duration = time_func([&]() { sd_begin(); });
    printf("SD begin(): %llu\n", duration);

    duration = time_func([&]() { fram.begin(); });
    printf("FRAM begin(): %llu\n\n", duration);

    printf("========== Sensors ==========\n\n");
    float val;
    float x, y, z;
    uint64_t sensors_total_sum = 0;
    uint64_t sum = 0;

    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { altimeter.read_data(&val, &val, 1); });
        sum += duration;
        sleep_ms(5);
    }
    sensors_total_sum += sum / NUM_RUNS;
    printf("Altimeter read_data(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { accel.read_accel(&x, &y, &z); });
        sum += duration;
        sleep_ms(5);
    }
    sensors_total_sum += sum / NUM_RUNS;
    printf("Accel read_accel(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { imu.read_accel(&x, &y, &z); });
        sum += duration;
        sleep_ms(5);
    }
    sensors_total_sum += sum / NUM_RUNS;
    printf("IMU read_accel(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { imu.read_gyro(&x, &y, &z); });
        sum += duration;
        sleep_ms(5);
    }
    sensors_total_sum += sum / NUM_RUNS;
    printf("IMU read_gyro(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { imu.read_orientation(&x, &y, &z); });
        sum += duration;
        sleep_ms(5);
    }
    sensors_total_sum += sum / NUM_RUNS;
    printf("IMU read_orientation(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { imu.read_gravity(&x, &y, &z); });
        sum += duration;
        sleep_ms(5);
    }
    sensors_total_sum += sum / NUM_RUNS;
    printf("IMU read_gravity(): %llu\n\n", sum / NUM_RUNS);

    printf("========== Storage ==========\n\n");
    uint8_t data;
    uint64_t storage_total_sum = 0;

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { sd_write(); });
        sum += duration;
        sleep_ms(5);
    }
    storage_total_sum += sum / NUM_RUNS;
    printf("SD write(): %llu\n", sum / NUM_RUNS);

    sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        duration = time_func([&]() { fram.read_bytes(0, &data, 1); });
        sum += duration;
        sleep_ms(5);
    }
    storage_total_sum += sum / NUM_RUNS;
    printf("FRAM read_bytes(): %llu\n", sum / NUM_RUNS);


    printf("=============================\n\n");

    printf("Sensors total sum: %llu\n", sensors_total_sum);
    printf("Storage total sum: %llu\n", storage_total_sum);

    return 0;
}