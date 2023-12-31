#ifndef STATE_HPP_
#define STATE_HPP_

#include "flight_mode.hpp"
#include <stdint.h>

#include "bno055.hpp"
#include "lis3dh.hpp"
#include "si7021.hpp"

#include "../sd/sd.hpp"

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
    namespace altimeter {
        extern bool init;
        extern float pressure;
        extern float altitude;
    } // namespace altimeter
    namespace gps {
        extern bool init;
        extern float latitude;
        extern float longitude;
        extern float altitude;
        extern uint8_t siv;
    } // namespace gps
    namespace imu {
        extern bool init;
        extern float mag_x;
        extern float mag_y;
        extern float mag_z;
        extern float gyro_x;
        extern float gyro_y;
        extern float gyro_z;

        extern BNO055 imu;
    } // namespace imu
    namespace accel {
        extern bool init;
        extern float accel_x;
        extern float accel_y;
        extern float accel_z;

        extern LIS3DH accel;
    } // namespace accel
    namespace therm {
        extern bool init;
        extern float temp;
        extern float humidity;

        extern Si7021 therm;
    } // namespace therm
    namespace sd {
        extern bool init;

        extern SD sd;
    } // namespace sd
    namespace rfm {
        extern bool init;
    }
}; // namespace state

#endif // STATE_HPP_