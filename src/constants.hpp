/**
 * @file constants.hpp
 * @author csg83, gb486
 *
 * @brief Constants and other definitions
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "blims.hpp"
#include "pico/stdlib.h"

/**
 * Container for flight-related constants
 */
namespace constants {

    constexpr uint i2c_baudrate = 400 * 1000;       // baud
    constexpr uint spi_baudrate = 20000000;         // baud
    constexpr uint sd_baudrate = 125 * 1000000 / 6; // baud
    constexpr uint rfm_baudrate = 115200;           // baud

    constexpr uint32_t watchdog_delay = 10000;      // ms

    constexpr uint8_t cycle_time = 50;              // ms

    constexpr uint8_t max_usb_failed_reads = 50;

    constexpr uint8_t channel_pt3 = 3;
    constexpr uint8_t channel_pt4 = 2;
    constexpr uint8_t channel_rtd = 1;

    constexpr uint mav_open_time = 7500;            // ms

    constexpr float arming_altitude = 1526;         // meters
    constexpr float main_deploy_altitude = 260;     // meters
    constexpr uint8_t max_failed_writes = 20;
    constexpr uint8_t max_failed_reads = 10;

    constexpr uint8_t ematch_threshold = 200;       // ms
    constexpr uint8_t sv_peak_threshold = 150;      // ms

    const uint16_t file_writes_threshold = 50000;   // cycles
    constexpr float log_threshold = 50;             // meters

    constexpr uint16_t main_deploy_wait = 160;      // cycles
    constexpr uint16_t main_log_shutoff = 30000;    // cycles

    constexpr uint8_t event_buffer_size = 32;       // bytes

    constexpr uint8_t rfm_packet_size = 106;        // bytes
    constexpr uint8_t umb_packet_size = 24;         // bytes

    constexpr BLIMSMode blims_mode = MVP_Flight;

}; // namespace constants

#endif // CONSTANTS_HPP