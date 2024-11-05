/**
 * @file constants.hpp
 * @author csg83
 *
 * @brief Constants and other definitions
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#ifdef VERBOSE
#define logf(...) printf(__VA_ARGS__)
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
    fram_write_fail = 16,

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

    umbilical_disconnected = 31,
};

enum class Command : char {
    launch = '0',
    toggle_mav = '1',
    toggle_sv = '2',

    clear_card = '3'
};

/**
 * Container for flight-related constants.
 */
namespace constants {

    constexpr uint i2c_baudrate = 400 * 1000;       // baud
    constexpr uint spi_baudrate = 20000000;         // baud
    constexpr uint rfm_baudrate = 57600;            // baud

    constexpr uint8_t cycle_time = 50;              // ms

    constexpr uint8_t max_usb_failed_reads = 50;

    constexpr float arming_altitude = 1526;         // meters
    constexpr float main_deploy_altitude = 260;     // meters
    constexpr uint8_t max_failed_reads = 10;

    constexpr uint8_t ematch_threshold = 200;       // ms

    const char *const boot_filename = "boot.txt";
    const uint16_t file_writes_threshold = 50000;   // cycles
    constexpr float log_threshold = 50;             // meters

    constexpr uint16_t main_deploy_wait = 160;      // cycles
    constexpr uint16_t main_log_shutoff = 30000;    // cycles

    // TODO: Remove
    constexpr float frequency = 922.2;              // MHz
    constexpr float bandwidth = 500;                // kHz
    constexpr uint8_t sf = 6;                       // Between 7 and 12
    constexpr uint8_t cr = 5;                       // Between 5 and 8. 4/8 coding ratio - one redundancy bit for every data bit
    constexpr uint8_t sw = 0x16;                    // Sync-word (defines network) Default is 0d18
    constexpr int8_t power = 17;                    // Between 2 and 17 or 20 for max power

    constexpr uint32_t interrupt_delay = 750;       // ms
    constexpr uint8_t packet_size = 86;             // bytes
}; // namespace constants

#endif // CONSTANTS_HPP