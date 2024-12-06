#ifndef UBLOX_MX_HPP
#define UBLOX_MX_HPP

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#include "ublox_defs.hpp"
#include <string>

#define UBLOX_ADDR (0x42)

/**
 * Representation of the U-blox GNSS receiver.
 */
class GNSS {
public:
    /**
     * Initializes a U-blox GNSS object on an I2C bus.
     * @param i2c_type The I2C bus that this sensor is on
     */

    GNSS(i2c_inst_t *i2c_type);

    /**
     * Attempts to establish a connection with the GNSS receiver.
     * @return True on successful connection, false otherwise
     */
    bool begin();

    bool read_data(gnss_data_t *data);

    bool send_config_msg(int config_msg);

private:
    bool parse_gngga(gnss_data_t *data);

    /**
     * Calculates a frame's UBX checksum, as defined in
     * section 3.4 of the interface description document
     * @param frame The UBX frame
     */
    void calc_ubx_checksum(UBXFrame *frame);

    bool send_ublox_frame(UBXFrame *frame);

    uint8_t *payload_cfg = nullptr;

    std::string raw_data = "";

    uint8_t buffer[256];
    
    /**
     * The I2C bus.
     */
    i2c_inst_t *i2c;
};

#endif // UBLOX_MX_HPP