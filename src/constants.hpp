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

#ifdef VERBOSE
#define logf(...) printf(__VA_ARGS__)
#define SPACER ("                        ")
#else
#define logf(...)
#endif

namespace command {
    constexpr char launch[] = "L";
    constexpr char mav_open[] = "M";
    constexpr char mav_close[] = "m";
    constexpr char sv_open[] = "S";
    constexpr char sv_close[] = "s";

    constexpr char safe[] = "V";

    constexpr char reset_card[] = "D";
    constexpr char reset_fram[] = "F";

    constexpr char reboot[] = "R";

    constexpr char change_target_lat[] = "C1";
    constexpr char change_target_long[] = "C2";
    constexpr char change_ref_pressure[] = "C3";
    constexpr char change_alt_state[] = "C4";
    constexpr char change_card_state[] = "C5";
    constexpr char change_alt_armed[] = "C6";
    constexpr char change_flight_mode[] = "C7";

}; // namespace command

/**
 * Container for flight-related constants
 */
namespace constants {

    constexpr uint i2c_baudrate = 400 * 1000;                    // baud
    constexpr uint spi_baudrate = 20000000;                      // baud
    constexpr uint sd_baudrate = 125 * 1000000 / 6;              // baud
    constexpr uint rfm_baudrate = 115200;                        // baud

    constexpr uint32_t watchdog_delay = 10000;                   // ms

    constexpr uint8_t cycle_time = 50;                           // ms

    constexpr uint8_t channel_pt3 = 3;
    constexpr uint8_t channel_pt4 = 2;
    constexpr uint8_t channel_rtd = 1;

    constexpr uint mav_open_time = 7500;                         // ms

    constexpr float arming_altitude = 1526;                      // meters
    constexpr float main_deploy_altitude = 260;                  // meters
    constexpr uint8_t max_failed_writes = 20;
    constexpr uint8_t max_failed_reads = 10;

    constexpr int gps_data_rate = 20;                            // Hz

    constexpr float pt_conversion_factor = 0.8998;
    constexpr float bv_conversion_factor = 3.3f / (1 << 12) * 3;

    constexpr uint8_t ematch_threshold = 200;                    // ms
    constexpr uint8_t sv_peak_threshold = 150;                   // ms
    constexpr uint16_t buzz_delay = 200;                         // ms

    const uint16_t file_writes_threshold = 50000;                // cycles
    constexpr float log_threshold = 50;                          // meters

    constexpr uint16_t main_deploy_wait = 160;                   // cycles
    constexpr uint16_t main_log_shutoff = 30000;                 // cycles

    constexpr uint32_t sync_word = 0x3E5D5967;
    constexpr uint8_t rfm_packet_size = 107;                     // bytes

    constexpr uint16_t min_umb_successful_reads = 100;
    constexpr uint16_t max_umb_failed_reads = 250;
    constexpr uint8_t umb_packet_size = 26;                      // bytes

    constexpr char command_start = '<';
    constexpr char command_stop = '>';

    constexpr BLIMSMode blims_mode = MVP_Flight;

}; // namespace constants

#endif // CONSTANTS_HPP