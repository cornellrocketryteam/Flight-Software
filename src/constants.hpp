#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace constants {
    namespace flight {
        constexpr int arming_altitude = 1524;      // Arming altitude (m)
        constexpr int main_deploy_altitude = 2286; // Main parachute deployment altitude (m)
    } // namespace flight
    namespace altimeter {
        constexpr float ref_pressure = 102106.3;   // Reference pressure (Pa)
    }
    namespace imu {
        constexpr float declination = 11.7;        // Duffield declination - change for launch
    }
    namespace sd {
        const char *const filename = "log.txt";      // The file to which data is logged
    }
}; // namespace constants

#endif // CONSTANTS_HPP_