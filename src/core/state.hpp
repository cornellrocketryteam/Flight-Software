#ifndef STATE_HPP_
#define STATE_HPP_

#include "flight_mode.hpp"
#include <stdint.h>
#include <vector>

#include "../sd/sd.hpp"

enum SensorState {
    OFF,
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

        extern int cycle_count;
        extern uint32_t timestamp;

        extern bool key_armed;
        extern bool altitude_armed;

        extern std::vector<std::string> events;
    } // namespace flight
    namespace alt {
        extern enum SensorState status;
        extern uint8_t failed_reads;

        extern float pressure;
        extern float altitude;

        extern float ref_pressure;
    } // namespace alt
    namespace gps {
        extern enum SensorState status;
        extern uint8_t failed_reads;

        extern float latitude;
        extern float longitude;
        extern float altitude;
        extern uint8_t siv;
    } // namespace gps
    namespace imu {
        extern enum SensorState status;
        extern uint8_t failed_reads;

        extern float mag_x;
        extern float mag_y;
        extern float mag_z;
        extern float gyro_x;
        extern float gyro_y;
        extern float gyro_z;

    } // namespace imu
    namespace accel {
        extern enum SensorState status;
        extern uint8_t failed_reads;

        extern float accel_x;
        extern float accel_y;
        extern float accel_z;

    } // namespace accel
    namespace therm {
        extern enum SensorState status;
        extern uint8_t failed_reads;

        extern float temp;

    } // namespace therm
    namespace sd {
        extern bool init;

    } // namespace sd
    namespace rfm {
        extern bool init;

    } // namespace rfm
}; // namespace state

#endif // STATE_HPP_