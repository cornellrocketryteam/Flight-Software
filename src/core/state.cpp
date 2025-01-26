/**
 * @file state.cpp
 * @author csg83, gb486
 *
 * @brief Rocket state variables
 */

#include "state.hpp"
#include "pins.hpp"

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

        uint32_t cycle_count = 0;
        uint32_t timestamp = 0;

        bool launch_commanded = false;

        uint16_t boot_count = 0;
        uint8_t watchdog_boot_count = 0;
        int old_mode = -1;

        int usb_failed_reads = 0;

        bool key_armed = false;
        bool alt_armed = false;

        std::vector<Event> events = {};

    } // namespace flight
    namespace alt {
        enum SensorState status = OFF;
        bool failed_init = false;
        uint8_t failed_reads = 0;

        float altitude = -1;
        float ref_pressure = -1;
        float temp = -1;
    } // namespace alt
    namespace gps {
        enum SensorState status = OFF;
        bool failed_init = false;
        uint8_t failed_reads = 0;

        bool valid = false;
        gnss_data_t data;
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
    namespace adc {
        enum SensorState status = OFF;
        bool failed_init = false;
        uint8_t failed_reads = 0;

        float pressure_pt3 = -1;
        float pressure_pt4 = -1;
        float temp_rtd = -1;

    } // namespace adc
    namespace sd {
        bool init = false;
        bool failed_init = false;
        uint8_t failed_writes = 0;

        int current_file = 0;

    } // namespace sd
    namespace fram {
        bool init = false;

    } // namespace fram
    namespace rfm {
        bool init = false;

    } // namespace rfm
    namespace mav {
        bool open = false;

    } // namespace mav
    namespace sv {
        bool open = true;

    } // namespace sv
    namespace blims {
        float motor_position = 0;
    } // namespace blims
}; // namespace state
