/**
 * @file constants.hpp
 * @author csg83, gb486
 *
 * @brief Constants and other definitions
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "pico/stdlib.h"

#ifdef VERBOSE
#define logf(...) printf(__VA_ARGS__)
#define SPACER ("                        ")
#else
#define logf(...)
#endif

/**
 * Container for flight-related constants
 */
namespace constants {

    // Peripheral baudrates
    constexpr uint i2c_baudrate = 400 * 1000;                    // baud
    constexpr uint spi_baudrate = 20000000;                      // baud
    constexpr uint sd_baudrate = 125 * 1000000 / 6;              // baud
    constexpr uint rfm_baudrate = 115200;                        // baud

    // Flight loop parameters
    constexpr uint32_t watchdog_delay = 10000;                   // ms
    constexpr uint8_t cycle_time = 50;                           // ms

    // Parachute deployment parameters
    constexpr float arming_altitude = 229;                       // meters
    constexpr float main_deploy_altitude = 457;                  // meters
    constexpr uint8_t ematch_threshold = 200;                    // ms
    constexpr uint8_t alt_sample_interval = 10;
    constexpr uint16_t main_deploy_wait = 160;                   // cycles

    // Sensor parameters
    constexpr uint8_t max_failed_writes = 20;
    constexpr uint8_t max_failed_reads = 20;
    constexpr int gps_data_rate = 20;                            // Hz
    constexpr float pt_conversion_factor = 0.8998;
    constexpr float bv_conversion_factor = 3.3f / (1 << 12) * 3;

    // ADC channel assignments
    constexpr uint8_t channel_pt3 = 3;
    constexpr uint8_t channel_pt4 = 2;
    constexpr uint8_t channel_rtd = 1;

    // Actuator parameters
    constexpr uint mav_open_time = 7880;                         // ms
    constexpr uint sv_open_delay_time = 60000;                   // ms
    constexpr uint16_t buzz_delay = 200;                         // ms

    // SD logging limits
    const uint16_t file_writes_threshold = 50000;                // cycles
    constexpr uint16_t main_log_shutoff = 30000;                 // cycles

    // Radio telemetry parameters
    constexpr uint32_t sync_word = 0x3E5D5967;                   // CRT!
    constexpr uint8_t rfm_packet_size = 107;                     // bytes

    // Umbilical telemetry parameters
    constexpr uint16_t min_umb_successful_reads = 100;
    constexpr uint16_t max_umb_failed_reads = 250;
    constexpr uint8_t umb_packet_size = 30;                      // bytes
    constexpr char command_start = '<';
    constexpr char command_stop = '>';

}; // namespace constants

#endif // CONSTANTS_HPP