/**
 * @file sensor.hpp
 * @author csg83
 *
 * @brief Sensor-related definitions
 */

#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "ads1015.hpp"
#include "bmp388.hpp"
#include "bno055.hpp"
#include "constants.hpp"
#include "lis3dh.hpp"
#include "ublox_mx.hpp"
#include <vector>
#ifdef SIM
#include "hitl.hpp"
#endif

class Altimeter {
public:
    Altimeter();
    void begin();
    void update_ref_pressure();
    void read_altitude();

private:
    BMP388 alt;
    float pressure;
    const float alpha = 0.1;
};

class GPS {
public:
    GPS();
    void begin();
    void read_data();

private:
    GNSS gnss;
};

class Accel {
public:
    Accel();
    void begin();
    void read_accel();

private:
    LIS3DH accel;
};

class IMU {
public:
    IMU();
    void begin();
    void read_gyro();
    void read_accel();
    void read_orientation();
    void read_gravity();

private:
    void failed_read();
    BNO055 imu;
};

class ADC {
public:
    ADC();
    void begin();
    void read_data();
    void read_internal();

private:
    ADS1015 adc;
    uint8_t channels[3] = {constants::channel_pt3, constants::channel_pt4, constants::channel_rtd};
    uint16_t data[3];

    uint16_t battery_voltage_raw;
};

#endif // SENSOR_HPP