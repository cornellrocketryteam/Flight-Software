#ifndef MODULES_HPP
#define MODULES_HPP

#include "bmp388.hpp"
#include "bno055.hpp"
#include "lis3dh.hpp"
#include "si7021.hpp"

#include "../rfm/rfm.hpp"
#include "../sd/sd.hpp"

enum Sensor {
    ALT,
    GPS,
    IMU,
    ACCEL,
    THERM
};

/**
 * Container for all sensor, SD card, and RFM objects.
 */
namespace modules {
    extern BMP388 altimeter;
    extern BNO055 imu;
    extern LIS3DH accel;
    extern Si7021 therm;

    extern SD sd;
    extern RFM rfm;
}; // namespace modules

#endif // MODULES_HPP