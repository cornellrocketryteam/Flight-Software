#ifndef MODULES_HPP
#define MODULES_HPP

#include "blims.hpp"
#include "bmp388.hpp"
#include "bno055.hpp"
#include "lis3dh.hpp"
#include "ublox_mx.hpp"

#include "../rfm/rfm.hpp"
#include "../sd/sd.hpp"

enum Sensor {
    ALT,
    GPS,
    IMU,
    ACCEL
};

/**
 * Container for all sensor, SD card, and RFM objects.
 */
namespace modules {
    extern BMP388 altimeter;
    extern BNO055 imu;
    extern LIS3DH accel;
    extern GNSS gps;

    extern SD sd;
    extern RFM rfm;
    extern BLIMS blims;
}; // namespace modules

#endif // MODULES_HPP