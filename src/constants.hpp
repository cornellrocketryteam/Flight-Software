#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

enum class Event : uint8_t {
    key_armed = 0,
    alt_armed = 1,

    alt_init_fail = 2,
    alt_read_fail = 3,
    alt_turn_off = 4,

    gps_init_fail = 5,
    gps_read_fail = 6,
    gps_turn_off = 7,

    imu_init_fail = 8,
    imu_read_fail = 9,
    imu_turn_off = 10,

    accel_init_fail = 11,
    accel_read_fail = 12,
    accel_turn_off = 13,

    therm_init_fail = 14,
    therm_read_fail = 15,
    therm_turn_off = 16,

    sd_init_fail = 17,
    sd_write_fail = 18,

    rfm_init_fail = 19,
    rfm_tx_fail = 20
};

/**
 * Container for flight-related constants.
 */
namespace constants {

    constexpr uint8_t cycle_time = 50;              // ms

    constexpr float arming_altitude = 1526;         // meters
    constexpr float main_deploy_altitude = 244;     // meters
    constexpr uint8_t max_failed_reads = 10;

    constexpr float accel_threshold = 4;            // G

    constexpr uint8_t ematch_threshold = 200;       // ms

    const char *const boot_filename = "boot.txt";
    const uint16_t file_writes_threshold = 50000;   // cycles
    constexpr float log_threshold = 50;             // meters

    constexpr float frequency = 915;                // MHz
    constexpr float bandwidth = 125;                // kHz
    constexpr uint8_t sf = 7;                       // Between 7 and 12
    constexpr uint8_t cr = 8;                       // Between 5 and 8. 4/8 coding ratio - one redundancy bit for every data bit
    constexpr uint8_t sw = 0x12;                    // Sync-word (defines network) Default is 18
    constexpr int8_t power = 20;                    // Between 2 and 17 or 20 for max power

    constexpr uint32_t interrupt_delay = 750;       // ms
    constexpr uint8_t packet_size = 86;             // bytes
}; // namespace constants

#endif // CONSTANTS_HPP