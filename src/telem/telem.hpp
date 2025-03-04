/**
 * @file telem.cpp
 * @author csg83
 *
 * @brief Telemetry-related definitions
 */

#ifndef TELEM_HPP
#define TELEM_HPP

#include "constants.hpp"
#include "pico/stdlib.h"

class Telem {
public:
    virtual void transmit() = 0;

protected:
    void pack_data();

    uint16_t packed_metadata = 0;
    uint32_t packed_events = 0;
};

class RFM : Telem {
public:
    void transmit();

private:
    uint8_t packet[constants::rfm_packet_size];
};

class Umbilical : Telem {
public:
    void transmit();

private:
    uint8_t packet[constants::umb_packet_size];
};

#endif // TELEM_HPP