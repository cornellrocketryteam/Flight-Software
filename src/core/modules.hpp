/**
 * @file modules.hpp
 * @author csg83
 *
 * @brief Definitions for sensor, actuator, storage and telem modules
 */

#ifndef MODULES_HPP
#define MODULES_HPP

#include "actuator.hpp"
#include "blims.hpp"
#include "fram.hpp"
#include "sd.hpp"
#include "sensor.hpp"
#include "telem.hpp"

extern Altimeter altimeter;
extern GPS gps;
extern Accel accel;
extern IMU imu;

extern SD sd;
extern FRAM fram;

extern RFM rfm;
extern Umbilical umb;

extern MAV mav;
extern SV sv;
extern SSA ssa;

extern BLIMS blims;

#endif // MODULES_HPP