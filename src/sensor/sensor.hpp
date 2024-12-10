/**
 * @file sensor.hpp
 * @author csg83
 *
 * @brief Sensor-related functionality
 */

#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "bmp388.hpp"
#include "bno055.hpp"
#include "lis3dh.hpp"
#include "ublox_mx.hpp"
#ifdef SIM
#include "hitl.hpp"
#endif

class Sensor {
public:
    virtual bool begin() = 0;

private:
};

class Altimeter : Sensor {
public:
    Altimeter();
    bool begin() override;
    void update_ref_pressure();
    void read_altitude();

private:
#ifndef SIM
    BMP388 alt;
#else
    HITL alt;
#endif
    float pressure;
    const float alpha = 0.1;
};

class GPS : Sensor {
public:
    GPS();
    bool begin() override;

private:
    GNSS gnss;
};

class Accel : Sensor {
public:
    Accel();
    bool begin() override;
    void read_accel();

private:
    LIS3DH accel;
};

class IMU : Sensor {
public:
    IMU();
    bool begin() override;
    void read_gyro();
    void read_accel();
    void read_orientation();
    void read_gravity();

private:
    BNO055 imu;
};

#endif // SENSOR_HPP