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
#include "../../src/constants.hpp"

#define BMP388_ADDR 0x77
#define I2C_PORT i2c0

#define CHIP_ID_ADDR 0x00
#define CHIP_ID_RET 0x50

#define CMD_READY_ADDR 0x03
#define CMD_READY_RET 0x10

#define CMD_ERROR_ADDR 0x02
#define CMD_ERROR_RET 0x7E

#define CMD_SOFT_RESET_ADDR 0x7E
#define CMD_SOFT_RESET 0xB6

#define SENSOR_STATUS 0x03

#define REG_DATA 0x04

bool write_register(const uint8_t addr, const uint8_t val) {
    uint8_t buf[2];
    buf[0] = addr;
    buf[1] = val;

    if (i2c_write_blocking(I2C_PORT, BMP388_ADDR, buf, 2, false) < 1) {
        return false;
    }
    return true;
}

bool read_register(const uint8_t addr, uint8_t *data, const uint8_t len) {
    if (i2c_write_blocking(I2C_PORT, BMP388_ADDR, &addr, 1, true) < 1) {
        return false;
    }
    if (i2c_read_blocking(I2C_PORT, BMP388_ADDR, data, len, false) < 1) {
        return false;
    }
    return true;
}

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    i2c_init(I2C_PORT, constants::i2c_baudrate);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    printf("========== BMP388::begin() ==========\n\n");
    
    sleep_ms(1000);
    printf("Reading chip ID...");
    uint8_t chip_id;
    if (read_register(CHIP_ID_ADDR, &chip_id, 1)) {
        if (chip_id == CHIP_ID_RET) {
            printf("success!\n");
        } else {
            printf("failed, received %d\n", chip_id);
        }
    } else {
        printf("transaction failed");
    }

    sleep_ms(1000);
    printf("Reading command ready...");
    uint8_t cmd_ready;
    if (read_register(CMD_READY_ADDR, &cmd_ready, 1)) {
        if (cmd_ready == CMD_READY_RET) {
            printf("success!\n");
        } else {
            printf("failed, received %d\n", chip_id);
        }
    } else {
        printf("transaction failed");
    }

    sleep_ms(1000);
    printf("Writing soft reset...");
    if (write_register(CMD_SOFT_RESET_ADDR, CMD_SOFT_RESET)) {
        printf("success!\n");
    } else {
        printf("transaction failed");
    }

    // sleep_ms(1000);
    // printf("Reading command error...");
    // uint8_t cmd_error;
    // if (read_register(CMD_ERROR_ADDR, &cmd_error, 1)) {
    //     if (cmd_error == CMD_ERROR_RET) {
    //         printf("success!\n");
    //     } else {
    //         printf("failed, received %d\n", cmd_error);
    //     }
    // } else {
    //     printf("transaction failed");
    // }

    sleep_ms(1000);

    printf("\n========== BMP388::read_altitude() ==========\n\n");

    uint8_t sensor_data[6] = {0};
    printf("Reading sensor data...");
    while (true) {
        if (read_register(REG_DATA, sensor_data, 6)) {
            printf("success! Data: ");
            for (int i = 0; i < 6; i++) {
                printf("%d ", sensor_data[i]);
            }
            printf("\n");
        } else {
            printf("transaction failed");
        }
        sleep_ms(1000);
    }

    return 0;
}