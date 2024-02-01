#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

/**
 * Container for flight-related constants.
 */
namespace constants {

    constexpr uint16_t arming_altitude = 25;        // meters           TODO: CHANGE TO 1524 FOR LAUNCH
    constexpr uint16_t main_deploy_altitude = 40;   // meters           TODO: CHANGE TO 2286 FOR LAUNCH
    constexpr uint8_t max_failed_reads = 10; 

    constexpr uint8_t accel_threshold = 3;          // m/s^2            TODO: CHANGE FOR LAUNCH

    const char *const filename = "log.txt";
}; // namespace constants

#endif // CONSTANTS_HPP_