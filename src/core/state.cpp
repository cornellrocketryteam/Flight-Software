/**
 * @file state.cpp
 * @author csg83, gb486
 *
 * @brief Rocket state variables
 */

#include "state.hpp"
#include "fram.hpp"
#include "pins.hpp"

class FlightMode;

EventBuffer events;

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

        uint32_t cycle_count = 0;
        uint32_t timestamp = 0;

        uint16_t boot_count = 0;
        uint8_t watchdog_boot_count = 0;
        uint8_t old_mode = -1;

        bool key_armed = false;
        bool alt_armed = false;
        bool safed = false;

    } // namespace flight
    namespace alt {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float altitude = -1;
        float ref_pressure = -1;
        float temperature = -1;
    } // namespace alt
    namespace gps {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        bool fresh = false;

        UbxNavPvt data = {};
        uint32_t unix_time = 0;
    } // namespace gps
    namespace imu {
        enum SensorState status = OFF;
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
        uint8_t failed_reads = 0;

        float accel_x = -1;
        float accel_y = -1;
        float accel_z = -1;

    } // namespace accel
    namespace adc {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float pressure_pt3 = -1;
        float pressure_pt4 = -1;
        float temp_rtd = -1;

        float battery_voltage = -1;

    } // namespace adc
    namespace sd {
        bool init = false;
        uint8_t failed_writes = 0;

        int current_file = 0;

    } // namespace sd
    namespace fram {
        bool init = false;
        uint16_t pt_index = static_cast<uint8_t>(Data::pt_index);

    } // namespace fram
    namespace umb {
        bool connected = false;
        bool launch_commanded = false;

    } // namespace umb
    namespace actuator {
        bool mav_open = false;
        bool sv_open = true;

    } // namespace actuator
    namespace blims {
        float motor_position = -1;

        float target_lat = -1;
        float target_long = -1;

    } // namespace blims
}; // namespace state
