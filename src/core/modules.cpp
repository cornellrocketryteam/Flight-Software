#include "modules.hpp"

namespace modules {
    BMP388 altimeter(I2C_PORT);
    BNO055 imu(I2C_PORT);
    LIS3DH accel(I2C_PORT);
    Si7021 therm(I2C_PORT);
    GNSS gps(I2C_PORT);

    SD sd;
    RFM rfm;
    BLIMS blims;
}; // namespace modules