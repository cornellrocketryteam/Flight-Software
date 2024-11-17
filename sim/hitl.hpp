/**
 * @file hitl.hpp
 * @author csg83
 * 
 * @brief The HITL software interface
 */

#ifndef HITL_HPP
#define HITL_HPP

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define HITL_ADDR (0x77)

class HITL {
public:
    HITL(i2c_inst_t *i2c_type);
    bool begin();
    bool read_pressure(float *pressure);
    bool read_altitude(float *altitude, float sea_level_pressure);
private:
    i2c_inst_t *i2c;
};

#endif // HITL_HPP