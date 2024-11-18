/**
 * @file hitl.cpp
 * @author csg83
 * 
 * @brief HITL software interface functionality
 */

#include "hitl.hpp"
#include <cstring>

HITL::HITL(i2c_inst_t *i2c_type) {
    i2c = i2c_type;
}

bool HITL::begin() {
    return true;
}

bool HITL::read_pressure(float *pressure) {
    return true;
}

bool HITL::read_altitude(float *altitude, float sea_level_pressure) {
    uint8_t data[4];
    uint32_t packed = 0;
    float temp;

    if (i2c_read_blocking(i2c, HITL_ADDR, data, 4, false) < 0) {
        return false;
    }

    packed = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
    temp = static_cast<float>(packed) / 1e6;
    *altitude = temp;

    return true;
}