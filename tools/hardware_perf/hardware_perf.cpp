/**
 * @file hardware_perf.cpp
 * @author csg83
 * 
 * @brief Records how long each hardware module takes to perform its action
 */

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
#include "ublox_mx.hpp"
#include "ads1015.hpp"
#include "mb85rs.hpp"

#include <functional>
#include <tuple>
#include <array>
#include <bitset>

#define NUM_RUNS 200

BMP388 altimeter(I2C_PORT);
BNO055 imu(I2C_PORT);
LIS3DH accel(I2C_PORT);
GNSS gps(I2C_PORT);
ADS1015 adc(I2C_PORT);

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

uint8_t packet[constants::rfm_packet_size];
char throwaway_buffer[200];

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

void rfm_transmit() {
    uart_write_blocking(UART_PORT, (const uint8_t *)packet, constants::rfm_packet_size);
}

void umb_transmit() {
    for (uint i = 0; i < constants::umb_packet_size; ++i) {
        sprintf(throwaway_buffer, "%s ", std::bitset<8>(static_cast<uint8_t>(packet[i])).to_string().c_str());
    }
}

void time_begin() {
    printf("============ Begin ============\n\n");
    printf("Altimeter begin(): %llu us\n", time_func([&]() { altimeter.begin(); }));
    printf("Accel begin(): %llu us\n", time_func([&]() { accel.begin(); }));
    printf("IMU begin(): %llu us\n", time_func([&]() { imu.begin(); }));
    printf("GPS begin(): %llu us\n", time_func([&]() { gps.begin(); }));
    printf("ADC begin(): %llu us\n\n", time_func([&]() { adc.begin(); }));
    printf("SD begin(): %llu us\n", time_func(sd_begin));
    printf("FRAM begin(): %llu us\n\n", time_func([&]() { fram.begin(); }));
}

uint64_t run_sensor_test(const char* sensor_name, std::function<void()> read_func) {
    uint64_t sum = 0;
    for (int i = 0; i < NUM_RUNS; i++) {
        sum += time_func(read_func);
        sleep_ms(5);
    }
    uint64_t average_duration = sum / NUM_RUNS;
    printf("%s: %llu us\n", sensor_name, average_duration);
    return average_duration;
}

uint64_t time_sensors() {
    printf("=========== Sensors ===========\n\n");
    uint64_t sensors_total_sum = 0;

    sensors_total_sum += run_sensor_test("Altimeter read_data()", [&]() { 
        float val;
        altimeter.read_data(&val, &val, 1); 
    });

    sensors_total_sum += run_sensor_test("Accel read_accel()", [&]() { 
        float x, y, z;
        accel.read_accel(&x, &y, &z); 
    });

    sensors_total_sum += run_sensor_test("GPS read_data()", [&]() { 
        gnss_data_t gps_data;
        gps.read_data(&gps_data); 
    });

    sensors_total_sum += run_sensor_test("IMU read_accel()", [&]() { 
        float x, y, z;
        imu.read_accel(&x, &y, &z); 
    });

    sensors_total_sum += run_sensor_test("IMU read_gyro()", [&]() { 
        float x, y, z;
        imu.read_gyro(&x, &y, &z); 
    });

    sensors_total_sum += run_sensor_test("IMU read_orientation()", [&]() { 
        float x, y, z;
        imu.read_orientation(&x, &y, &z); 
    });

    sensors_total_sum += run_sensor_test("IMU read_gravity()", [&]() { 
        float x, y, z;
        imu.read_gravity(&x, &y, &z); 
    });

    sensors_total_sum += run_sensor_test("ADC read_data()", [&]() { 
        uint8_t adc_channels[] = {1, 2, 3};
        uint16_t adc_data[3];
        adc.read_data(adc_channels, sizeof(adc_channels), adc_data); 
    });

    printf("\n");
    return sensors_total_sum;
}

uint64_t time_storage() {
    printf("=========== Storage ===========\n\n");
    uint64_t storage_total_sum = 0;

    storage_total_sum += run_sensor_test("SD write()", [&]() { sd_write(); });

    storage_total_sum += run_sensor_test("FRAM read_bytes()", [&]() { 
        uint8_t data;
        fram.read_bytes(0, &data, 1); 
    });

    printf("\n");
    return storage_total_sum;
}

uint64_t time_telem() {
    printf("============ Telem ============\n\n");
    uint64_t telem_total_sum = 0;

    telem_total_sum += run_sensor_test("RFM transmit()", [&]() { rfm_transmit(); });
    telem_total_sum += run_sensor_test("UMB transmit()", [&]() { umb_transmit(); });

    printf("\n");
    return telem_total_sum;
}

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }

    printf("Connected, NUM_RUNS = %d\n", NUM_RUNS);
    init_pins();

    time_begin();

    uint64_t sensors_total_sum = time_sensors();
    uint64_t storage_total_sum = time_storage();
    uint64_t telem_total_sum = time_telem();

    printf("===============================\n\n");

    printf("Sensors total sum: %llu us\n", sensors_total_sum);
    printf("Storage total sum: %llu us\n", storage_total_sum);
    printf("Telem total sum: %llu us\n", telem_total_sum);

    return 0;
}