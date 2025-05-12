/**
 * @file modules.hpp
 * @author csg83
 *
 * @brief Definitions for sensor, actuator, storage and telem modules
 */

#include "modules.hpp"

Altimeter altimeter;
GPS gps;
Accel accel;
IMU imu;
ADC adc;

SD sd;
FRAM fram;

RFM rfm;
Umbilical umb;

MAV mav;
SV sv;
SSA ssa;
Buzzer buzzer;

#ifdef USE_BLIMS
BLIMS blims_obj;
#endif