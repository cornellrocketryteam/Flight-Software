#ifndef STATE_HPP
#define STATE_HPP

// clang-format off
#include "flight_mode.hpp"
#include "../constants.hpp"
#include <stdint.h>
#include <vector>
// clang-format on

#include "../sd/sd.hpp"

enum SensorState {
    OFF = 0,
    VALID,
    INVALID
};

/**
 * Container for the current flight state. Contains sensor data,
 * sensor statuses, and flight level data.
 */
namespace state {
    namespace flight {
        extern StartupMode startup_class;
        extern StandbyMode standby_class;
        extern AscentMode ascent_class;
        extern DrogueDeployedMode drogue_deployed_class;
        extern MainDeployedMode main_deployed_class;
        extern FaultMode fault_class;

        extern FlightMode *startup;
        extern FlightMode *standby;
        extern FlightMode *ascent;
        extern FlightMode *drogue_deployed;
        extern FlightMode *main_deployed;
        extern FlightMode *fault;

        extern FlightMode *mode;

        extern uint32_t cycle_count;
        extern uint32_t timestamp;

        extern int boot_count;
        extern int old_mode;

        extern bool key_armed;
        extern bool alt_armed;

        extern uint32_t ematch_start;

        extern std::vector<Event> events;
        // blims add
        extern uint32_t hold_start;
    } // namespace flight
    namespace alt {
        extern enum SensorState status;
        extern bool failed_init;
        extern uint8_t failed_reads;

        extern float pressure;
        extern float altitude;

        extern float ref_pressure;
    } // namespace alt
    namespace gps {
        extern enum SensorState status;
        extern bool failed_init;
        extern uint8_t failed_reads;

        extern gnss_data_t data;

    } // namespace gps
    namespace imu {
        extern enum SensorState status;
        extern bool failed_init;
        extern uint8_t failed_reads;

        extern float accel_x;
        extern float accel_y;
        extern float accel_z;

        extern float gyro_x;
        extern float gyro_y;
        extern float gyro_z;

        extern float orientation_x;
        extern float orientation_y;
        extern float orientation_z;

        extern float gravity_x;
        extern float gravity_y;
        extern float gravity_z;

    } // namespace imu
    namespace accel {
        extern enum SensorState status;
        extern bool failed_init;
        extern uint8_t failed_reads;

        extern float accel_x;
        extern float accel_y;
        extern float accel_z;

    } // namespace accel
    namespace therm {
        extern enum SensorState status;
        extern bool failed_init;
        extern uint8_t failed_reads;

        extern float temp;

    } // namespace therm
    namespace sd {
        extern bool init;
        extern bool failed_init;

        extern int current_file;

    } // namespace sd
    namespace rfm {
        extern bool init;
        extern bool attempted_init;
        extern uint32_t start_time;

    } // namespace rfm
    namespace blims {
        extern int32_t curr_action_duration;
        extern int curr_action_index;
    } // namespace blims
}; // namespace state

#endif // STATE_HPP