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
        bool alt_armed = false;

        uint32_t ematch_start = 0;

        std::vector<Event> events = {};
    } // namespace flight
    namespace alt {
        enum SensorState status = OFF;
        bool failed_init = false;
        uint8_t failed_reads = 0;

        float pressure = -1;
        float altitude = -1;

        float ref_pressure = -1;
    } // namespace alt
    namespace gps {
        enum SensorState status = OFF;
        bool failed_init = false;
        uint8_t failed_reads = 0;

        bool valid = false;
        int32_t latitude = -1;
        int32_t longitude = -1;
        int32_t altitude = -1;
        uint8_t siv = -1;
    } // namespace gps
    namespace imu {
        enum SensorState status = OFF;
        bool failed_init = false;
        uint8_t failed_reads = 0;

        float accel_x = -1;
        float accel_y = -1;
        float accel_z = -1;

        float gyro_x = -1;
        float gyro_y = -1;
        float gyro_z = -1;

        float orientation_x = -1;
        float orientation_y = -1;
        float orientation_z = -1;

        float gravity_x = -1;
        float gravity_y = -1;
        float gravity_z = -1;

    } // namespace imu
    namespace accel {
        enum SensorState status = OFF;
        bool failed_init = false;
        uint8_t failed_reads = 0;

        float accel_x = -1;
        float accel_y = -1;
        float accel_z = -1;

    } // namespace accel
    namespace therm {
        enum SensorState status = OFF;
        bool failed_init = false;
        uint8_t failed_reads = 0;

        float temp = -1;

    } // namespace therm
    namespace sd {
        bool init = false;
        bool failed_init = false;
        FIL file;

    } // namespace sd
    namespace rfm {
        bool init = false;
        bool attempted_init = false;

    } // namespace rfm
}; // namespace state
