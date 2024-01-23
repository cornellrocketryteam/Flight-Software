#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

/**
 * Container for flight-related constants.
 */
namespace constants {
    namespace flight {
        constexpr int arming_altitude = 1524;      // Arming altitude (m)
        constexpr int main_deploy_altitude = 2286; // Main parachute deployment altitude (m)
        constexpr uint8_t max_failed_reads = 10;   // The number of times a sensor reading can fail before shutdown
    } // namespace flight
    namespace alt {
        constexpr double ref_pressure = 1013.25;   // Reference pressure (hPa)
    }
    namespace imu {
        constexpr double declination = 11.7;        // Duffield declination - change for launch
    }
    namespace sd {
        const char *const filename = "log.txt";      // The file to which data is logged
    }
}; // namespace constants

#endif // CONSTANTS_HPP_