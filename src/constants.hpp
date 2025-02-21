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

enum class Event : uint8_t {
    alt_armed = 0,

    alt_init_fail = 1,
    alt_read_fail = 2,

    gps_init_fail = 3,
    gps_read_fail = 4,

    imu_init_fail = 5,
    imu_read_fail = 6,

    accel_init_fail = 7,
    accel_read_fail = 8,

    adc_init_fail = 9,
    adc_read_fail = 10,

    fram_init_fail = 11,
    fram_read_fail = 12,
    fram_write_fail = 13,

    sd_init_fail = 14,
    sd_write_fail = 15,

    mav_actuated = 16,
    sv_actuated = 17,

    main_deploy_wait_end = 18,
    main_log_shutoff = 19,
    cycle_overflow = 20,

    unknown_command_received = 21,
    launch_command_received = 22,
    mav_command_received = 23,
    sv_command_received = 24,
    safe_command_received = 25,
    reset_card_command_received = 26,
    reset_fram_command_received = 27,
    state_change_command_received = 28,

    umbilical_disconnected = 29
};

enum class Command : char {
    launch = '0',
    mav_open = '1',
    mav_close = '2',
    sv_open = '3',
    sv_close = '4',

    safe = '5',

    reset_card = '6',
    reset_fram = '7',

    // TODO: State change command
};

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

    constexpr uint8_t rfm_packet_size = 106;        // bytes
    constexpr uint8_t umb_packet_size = 24;         // bytes

    constexpr BLIMSMode blims_mode = MVP_Flight;

}; // namespace constants

#endif // CONSTANTS_HPP