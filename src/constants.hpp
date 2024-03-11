#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

enum class Event : uint8_t {
    key_armed = 0,
    alt_armed = 1,

    alt_init_fail = 2,
    alt_read_fail = 3,
    alt_turn_off = 4,

    imu_init_fail = 5,
    imu_read_fail = 6,
    imu_turn_off = 7,

    accel_init_fail = 8,
    accel_read_fail = 9,
    accel_turn_off = 10,

    therm_init_fail = 11,
    therm_read_fail = 12,
    therm_turn_off = 13,

    sd_init_fail = 14,
    sd_write_fail = 15,

    rfm_init_fail = 16,
    rfm_tx_fail = 17
};

/**
 * Container for flight-related constants.
 */
namespace constants {

    constexpr uint16_t arming_altitude = 25;        // meters           TODO: CHANGE TO 1524 FOR LAUNCH
    constexpr uint16_t main_deploy_altitude = 40;   // meters           TODO: CHANGE TO 2286 FOR LAUNCH
    constexpr uint8_t max_failed_reads = 10;

    constexpr uint8_t accel_threshold = 2;          // G            TODO: CHANGE TO 5 FOR LAUNCH

    constexpr uint8_t ematch_threshold = 200;       // ms

    const char *const log_filename = "log.txt";

    constexpr uint8_t packet_size = 86;
}; // namespace constants

#endif // CONSTANTS_HPP_