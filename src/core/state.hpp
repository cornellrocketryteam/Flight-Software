#ifndef STATE_HPP_
#define STATE_HPP_

#include "flight_mode.hpp"
#include <stdint.h>

#include "../sd/sd.hpp"

/**
 * Container for the current flight state. Contains sensor data,
 * sensor statuses, and flight level data.
 */
namespace state {
    namespace flight {
        extern StartupMode startup_class;
        extern StandbyMode standby_class;
        extern PoweredAscentMode powered_ascent_class;
        extern CoastingMode coasting_class;
        extern DrogueDeployedMode drogue_deployed_class;
        extern MainDeployedMode main_deployed_class;

        extern FlightMode *startup;
        extern FlightMode *standby;
        extern FlightMode *powered_ascent;
        extern FlightMode *coasting;
        extern FlightMode *drogue_deployed;
        extern FlightMode *main_deployed;

        extern FlightMode *mode;

        extern int cycle_count;

        extern bool key_armed;
        extern bool altitude_armed;
    } // namespace flight
    namespace alt {
        extern bool init;
        extern double pressure;
        extern double altitude;

    } // namespace alt
    namespace gps {
        extern bool init;
        extern float latitude;
        extern float longitude;
        extern float altitude;
        extern uint8_t siv;
    } // namespace gps
    namespace imu {
        extern bool init;
        extern double mag_x;
        extern double mag_y;
        extern double mag_z;
        extern double gyro_x;
        extern double gyro_y;
        extern double gyro_z;

    } // namespace imu
    namespace accel {
        extern bool init;
        extern double accel_x;
        extern double accel_y;
        extern double accel_z;

    } // namespace accel
    namespace therm {
        extern bool init;
        extern double temp;
        extern double humidity;

    } // namespace therm
    namespace sd {
        extern bool init;

    } // namespace sd
    namespace rfm {
        extern bool init;

    } // namespace rfm
}; // namespace state

#endif // STATE_HPP_