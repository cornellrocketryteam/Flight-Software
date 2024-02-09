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
        FaultMode fault_class;

        FlightMode *startup = &startup_class;
        FlightMode *standby = &standby_class;
        FlightMode *ascent = &ascent_class;
        FlightMode *drogue_deployed = &drogue_deployed_class;
        FlightMode *main_deployed = &main_deployed_class;
        FlightMode *fault = &fault_class;

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

        float pressure = -1;
        float altitude = -1;

        float ref_pressure = -1;
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

        float mag_x = -1;
        float mag_y = -1;
        float mag_z = -1;
        float gyro_x = -1;
        float gyro_y = -1;
        float gyro_z = -1;

    } // namespace imu
    namespace accel {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float accel_x = -1;
        float accel_y = -1;
        float accel_z = -1;

    } // namespace accel
    namespace therm {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float temp = -1;

    } // namespace therm
    namespace sd {
        bool init = false;
        FIL file;

    } // namespace sd
    namespace rfm {
        bool init = false;

    } // namespace rfm
}; // namespace state
