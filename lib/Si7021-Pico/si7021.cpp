#include "si7021.hpp"
#include <cstdio>

Si7021::Si7021(i2c_inst_t *i2c_type) {
    i2c = i2c_type;
}

bool Si7021::begin() {
    if (!reset()) {
        return false;
    }

    sleep_ms(100);

    u_int8_t buf[8];
    get_id(buf);

    if (buf[4] != SI7021_DEVICE_ID) {
        fprintf(stderr, "Incorrect device ID: %d\n", buf[4]);
        return false;
    }
    return true;
}

float Si7021::read_temperature() {
    u_int8_t temp_cmd[1] = { SI7021_CMD_TEMP_NO_HOLD };
    uint8_t temp_buf[2];

    i2c_write_blocking(i2c, SI7021_ADDR, temp_cmd, 1, true);
    sleep_ms(20);
    i2c_read_blocking(i2c, SI7021_ADDR, temp_buf, 2, false);

    uint16_t temp_int = (temp_buf[0] << 8) | temp_buf[1];

    return (175.72 * (float)temp_int / 65536) - 46.85;
}

float Si7021::read_humidity() {
    u_int8_t hum_cmd[1] = { SI7021_CMD_HUM_NO_HOLD };
    uint8_t hum_buf[2];

    i2c_write_blocking(i2c, SI7021_ADDR, hum_cmd, 1, true);
    sleep_ms(20);
    i2c_read_blocking(i2c, SI7021_ADDR, hum_buf, 2, false);

    uint16_t hum_int = (hum_buf[0] << 8) | hum_buf[1];

    return (125 * (float)hum_int / 65536) - 6;
}

void Si7021::get_id(uint8_t *buf) {
    u_int8_t buf_id_0[2] = { SI7021_CMD_ID_0_0, SI7021_CMD_ID_0_1 };
    u_int8_t buf_id_1[2] = { SI7021_CMD_ID_1_0, SI7021_CMD_ID_1_1 };

    u_int8_t read_buf_0[4], read_buf_1[4];

    i2c_write_blocking(i2c, SI7021_ADDR, buf_id_0, 2, true);
    i2c_read_blocking(i2c, SI7021_ADDR, read_buf_0, 4, false);

    for (int i = 0; i < 4; i++) {
        buf[i] = read_buf_0[i];
        // printf("buf[%d]: %d\n", i, read_buf_0[i]);
    }

    i2c_write_blocking(i2c, SI7021_ADDR, buf_id_1, 2, true);
    i2c_read_blocking(i2c, SI7021_ADDR, read_buf_1, 4, false);

    for (int i = 0; i < 4; i++) {
        buf[i+4] = read_buf_1[i];
        // printf("buf[%d]: %d\n", i, read_buf_1[i]);
    }
}

bool Si7021::reset() {
    uint8_t buf[1] = { SI7021_CMD_RESET };
    int ret = i2c_write_blocking(i2c, SI7021_ADDR, buf, 1, false);

    if (ret < 1) {
        fprintf(stderr, "Chip reset failed\n");
        return false;
    }
    return true;
}