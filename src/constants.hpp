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

    constexpr uint16_t arming_altitude = 10;        // meters           TODO: CHANGE TO 305 FOR LAUNCH
    constexpr uint16_t main_deploy_altitude = 25;   // meters           TODO: CHANGE FOR LAUNCH
    constexpr uint8_t max_failed_reads = 10;

    constexpr uint8_t accel_threshold = 2;          // G            TODO: CHANGE TO 3 FOR LAUNCH

    constexpr uint8_t ematch_threshold = 200;       // ms

    const char *const boot_filename = "boot.txt";
    const uint16_t file_writes_threshold = 50000;

    constexpr uint8_t packet_size = 86;
}; // namespace constants

#endif // CONSTANTS_HPP