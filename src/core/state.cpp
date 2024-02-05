#include "state.hpp"
#include "../pins.hpp"

#include <stdint.h>

class FlightMode;

namespace state {
    namespace flight {
        StartupMode startup_class;
        StandbyMode standby_class;
        AscentMode ascent_class;
        DrogueDeployedMode drogue_deployed_class;
        MainDeployedMode main_deployed_class;

        FlightMode *startup = &startup_class;
        FlightMode *standby = &standby_class;
        FlightMode *ascent = &ascent_class;
        FlightMode *drogue_deployed = &drogue_deployed_class;
        FlightMode *main_deployed = &main_deployed_class;

        FlightMode *mode = startup;

        int cycle_count = 0;
        uint32_t timestamp = 0;

        bool key_armed = false;
        bool altitude_armed = false;

        std::vector<std::string> events = {};
    } // namespace flight
    namespace alt {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        double pressure = -1;
        double altitude = -1;

        double ref_pressure = -1;
    } // namespace alt
    namespace gps {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float latitude = -1;
        float longitude = -1;
        float altitude = -1;
        uint8_t siv = -1;
    } // namespace gps
    namespace imu {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        double mag_x = -1;
        double mag_y = -1;
        double mag_z = -1;
        double gyro_x = -1;
        double gyro_y = -1;
        double gyro_z = -1;

    } // namespace imu
    namespace accel {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        double accel_x = -1;
        double accel_y = -1;
        double accel_z = -1;

    } // namespace accel
    namespace therm {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        double temp = -1;

    } // namespace therm
    namespace sd {
        bool init = false;
        FIL file;

    } // namespace sd
    namespace rfm {
        bool init = false;

    } // namespace rfm
}; // namespace state
