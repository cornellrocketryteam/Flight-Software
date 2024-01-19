#ifndef MODULES_HPP_
#define MODULES_HPP_

#include "bmp388.hpp"
#include "bno055.hpp"
#include "lis3dh.hpp"
#include "si7021.hpp"

#include "../rfm/rfm.hpp"
#include "../sd/sd.hpp"

namespace modules {
    extern BMP388 altimeter;
    extern BNO055 imu;
    extern LIS3DH accel;
    extern Si7021 therm;

    extern SD sd;
    extern RFM rfm;
}; // namespace modules

#endif