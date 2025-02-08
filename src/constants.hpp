#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "blims.hpp"
#include <cstdint>

#ifdef VERBOSE
#define logf(...) printf(__VA_ARGS__)
#else
#define logf(...)
#endif

enum class Event : uint8_t {
    alt_armed = 0,

    alt_init_fail = 1,
    alt_read_fail = 2,
    alt_turn_off = 3,

    gps_init_fail = 4,
    gps_read_fail = 5,
    gps_turn_off = 6,

    imu_init_fail = 7,
    imu_read_fail = 8,
    imu_turn_off = 9,

    accel_init_fail = 10,
    accel_read_fail = 11,
    accel_turn_off = 12,

    therm_init_fail = 13,
    therm_read_fail = 14,
    therm_turn_off = 15,

    sd_init_fail = 16,
    sd_write_fail = 17,

    rfm_init_fail = 18,
    rfm_tx_fail = 19,

    main_deploy_wait_end = 20,
    main_log_shutoff = 21,

    below_brake_alt = 22,

};

/**
 * Container for flight-related constants.
 */
namespace constants {

    constexpr uint8_t cycle_time = 50;              // ms

    constexpr float arming_altitude = 305;          // meters
    constexpr float main_deploy_altitude = 610;     // meters
    constexpr uint8_t max_failed_reads = 10;

    constexpr float accel_threshold = 4;            // G

    constexpr uint8_t ematch_threshold = 200;       // ms

    const char *const boot_filename = "boot.txt";
    const uint16_t file_writes_threshold = 50000;   // cycles
    constexpr float log_threshold = 50;             // meters

    constexpr uint16_t main_deploy_wait = 160;      // cycles
    constexpr uint16_t main_log_shutoff = 30000;    // cycles

    constexpr float frequency = 922.2;              // MHz
    constexpr float bandwidth = 125;                // kHz
    constexpr uint8_t sf = 7;                       // Between 7 and 12
    constexpr uint8_t cr = 8;                       // Between 5 and 8. 4/8 coding ratio - one redundancy bit for every data bit
    constexpr uint8_t sw = 0x16;                    // Sync-word (defines network) Default is 0d18
    constexpr int8_t power = 20;                    // Between 2 and 17 or 20 for max power

    constexpr uint32_t interrupt_delay = 750;       // ms
    constexpr uint8_t packet_size = 86;             // bytes

    constexpr BLIMSMode blims_mode = MVP_Flight;

}; // namespace constants

#endif // CONSTANTS_HPP