/**
 * @file state.hpp
 * @author csg83, gb486
 *
 * @brief Rocket state variables
 */

#ifndef STATE_HPP
#define STATE_HPP

// clang-format off
#include "flight_mode.hpp"
#include "ublox_nav_pvt.hpp"
#include "sensor.hpp"
#include "constants.hpp"
#include "sd.hpp"
#include <stdint.h>
#include "util.hpp"
// clang-format on

extern EventBuffer events;

/**
 * Container for the current flight state. Contains sensor data,
 * sensor statuses, and flight level data.
 */
struct FlightState {
    struct {
        uint8_t mode_id = 0;
        uint32_t cycle_count = 0;
        uint32_t timestamp = 0;

        bool alt_armed = false;

        // TODO: Move these
        bool key_armed = false;
        int usb_failed_reads = 0;
        uint16_t boot_count = 0;
        uint8_t watchdog_boot_count = 0;
        int old_mode = -1;

        bool launch_commanded = false;

    } flight;
    struct {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float altitude = -1;
        float temp = -1;
        float ref_pressure = -1;
    } alt;
    struct {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        // TODO struct
        bool fresh = false;
    } gps;
    struct {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float accel_x = -1;
        float accel_y = -1;
        float accel_z = -1;

        float gyro_x = -1;
        float gyro_y = -1;
        float gyro_z = -1;

        float orientation_x = -1;
        float orientation_y = -1;
        float orientation_z = -1;

        float gravity_x = -1;
        float gravity_y = -1;
        float gravity_z = -1;
    } imu;
    struct {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float accel_x = -1;
        float accel_y = -1;
        float accel_z = -1;
    } accel;
    struct {
        enum SensorState status = OFF;
        uint8_t failed_reads = 0;

        float pressure_pt3 = -1;
        float pressure_pt4 = -1;
        float temp_rtd = -1;

        float battery_voltage;
    } adc;
    struct {
        bool init = false;
        uint8_t failed_writes = 0;

        int current_file = 0;
    } sd;
    struct {
        bool init = false;

        uint8_t pt_index = 0;
    } fram;
    struct {
        bool mav_open = false;
        bool sv_open = true;
    } actuator;
    struct {
        float motor_position = -1;
    } blims;
};

extern FlightState *state;
extern FlightState *storage_state;

#endif // STATE_HPP