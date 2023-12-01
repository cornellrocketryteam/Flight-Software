#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

namespace constants {
    namespace flight {
        constexpr int arming_altitude = 1524; // Arming altitude (m)
    }
    namespace altimeter {
        constexpr float ref_pressure = 102106.3; // Reference pressure (Pa)
    }
    namespace imu {
        constexpr float declination = 11.7; // Duffield declination - change for launch
    }
};

#endif // CONSTANTS_HPP_