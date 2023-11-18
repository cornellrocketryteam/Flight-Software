#include "pico/stdlib.h"
#include "hardware/i2c.h"

#ifndef SI7021_
#define SI7021_

#define SI7021_ADDR (0x40)

#define SI7021_DEVICE_ID (0x15)

#define SI7021_CMD_HUM_HOLD (0xE5)
#define SI7021_CMD_HUM_NO_HOLD (0xF5)

#define SI7021_CMD_TEMP_HOLD (0xE3)
#define SI7021_CMD_TEMP_NO_HOLD (0xF3)

#define SI7021_CMD_RESET (0xFE)

#define SI7021_CMD_ID_0_0 (0xFA)
#define SI7021_CMD_ID_0_1 (0x0F)

#define SI7021_CMD_ID_1_0 (0xFC)
#define SI7021_CMD_ID_1_1 (0xC9)

class Si7021 {
public:
    Si7021(i2c_inst_t *i2c_type);
    bool begin();
    float read_temperature();
    float read_humidity();

    void get_id(uint8_t *buf);
    bool reset();

private:
    i2c_inst_t *i2c;
};


#endif