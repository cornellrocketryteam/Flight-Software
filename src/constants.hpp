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

    therm_init_fail = 9,
    therm_read_fail = 10,

    voltage_init_fail = 11,
    voltage_read_fail = 12,

    adc_init_fail = 13,
    adc_read_fail = 14,

    fram_init_fail = 15,
    fram_rw_fail = 16,

    sd_init_fail = 17,
    sd_write_fail = 18,

    mav_actuated = 19,
    sv_actuated = 20,

    main_deploy_wait_end = 21,
    main_log_shutoff = 22,
    cycle_overflow = 23,

    rfm_init_fail = 24,
    rfm_tx_fail = 25,

    launch_command_received = 26,
    mav_command_received = 27,
    sv_command_received = 28,
    clear_card_command_received = 29,
    unknown_command_received = 30,

    umbilical_disconnected = 31
};

enum class Command : char {
    launch = '0',
    mav_open = '1',
    mav_close = '2',
    sv_open = '3',
    sv_close = '4',

    clear_card = '5'
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

    constexpr uint mav_open_time = 15000;           // ms

    constexpr float arming_altitude = 1526;         // meters
    constexpr float main_deploy_altitude = 260;     // meters
    constexpr uint8_t max_failed_reads = 10;

    constexpr uint8_t ematch_threshold = 200;       // ms

    const uint16_t file_writes_threshold = 50000;   // cycles
    constexpr float log_threshold = 50;             // meters

    constexpr uint16_t main_deploy_wait = 160;      // cycles
    constexpr uint16_t main_log_shutoff = 30000;    // cycles

    constexpr uint8_t rfm_packet_size = 107;        // bytes
    constexpr uint8_t umb_packet_size = 28;         // bytes

    // BLIMS Constants
    constexpr float brake_alt = 10; // To be updated for when we want BLiMS to brake
    constexpr uint32_t initial_hold_threshold = 10000;
    constexpr uint32_t turn_hold_threshold = 5000;
    constexpr uint32_t neutral_hold_threshold = 2000;
    constexpr float neutral_pos = 0.5;

}; // namespace constants

#endif // CONSTANTS_HPP