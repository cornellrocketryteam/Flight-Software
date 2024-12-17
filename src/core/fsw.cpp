/**
 * @file fsw.cpp
 * @author csg83
 *
 * @brief The main flight loop
 */

#include "fsw.hpp"
#include "hardware/watchdog.h"

void Flight::execute() {

    logf("---------------BEGIN LOOP %d---------------\n", state::flight::cycle_count);
    logf("Time: %d ms\n", state::flight::timestamp);
    logf("Mode: %s\n", state::flight::mode->name().c_str());
    logf("Altitude armed: %d\n\n", state::flight::alt_armed);

    logf("ALTIMETER: %d\n", state::alt::status);
    logf("Altitude (m): %.3f\n", state::alt::altitude);
    logf("Temperature (C): %.3f\n\n", state::alt::temp);

    logf("GPS: %d, Message: %d\n", state::gps::status, state::gps::data.valid);
    logf("Latitude: %f\n", state::gps::data.latitude);
    logf("Longitude: %f\n", state::gps::data.longitude);
    logf("Num Satellites: %d\n\n", state::gps::data.num_satellites);

    logf("IMU: %d\n", state::imu::status);
    logf("Gyro X (deg/s): %.3f\n", state::imu::gyro_x);
    logf("Gyro Y (deg/s): %.3f\n", state::imu::gyro_y);
    logf("Gyro Z (deg/s): %.3f\n\n", state::imu::gyro_z);

    logf("Accel X (m/s^2): %.3f\n", state::imu::accel_x);
    logf("Accel Y (m/s^2): %.3f\n", state::imu::accel_y);
    logf("Accel Z (m/s^2): %.3f\n\n", state::imu::accel_z);

    logf("Orientation X (deg): %.3f\n", state::imu::orientation_x);
    logf("Orientation Y (deg): %.3f\n", state::imu::orientation_y);
    logf("Orientation Z (deg): %.3f\n\n", state::imu::orientation_z);

    logf("Gravity X (m/s^2): %.3f\n", state::imu::gravity_x);
    logf("Gravity Y (m/s^2): %.3f\n", state::imu::gravity_y);
    logf("Gravity Z (m/s^2): %.3f\n\n", state::imu::gravity_z);

    logf("ACCELEROMETER: %d\n", state::accel::status);
    logf("Accel X (G): %.3f\n", state::accel::accel_x);
    logf("Accel Y (G): %.3f\n", state::accel::accel_y);
    logf("Accel Z (G): %.3f\n\n", state::accel::accel_z);

    logf("ADC: %d\n", state::adc::status);
    logf("PT 3 (kPa): %.3f\n", state::adc::pressure_pt3);
    logf("PT 4 (kPa): %.3f\n", state::adc::pressure_pt4);
    logf("RTD (C): %.3f\n\n", state::adc::temp_rtd);

    logf("SD: %d\n", state::sd::init);
    logf("FRAM: %d\n\n", state::fram::init);

    cycle_start = to_ms_since_boot(get_absolute_time());

    // Execute the current flight mode
    state::flight::mode->execute();
    state::flight::mode->transition();

    // Sleep until the fixed cycle time is reached
    cycle_duration = to_ms_since_boot(get_absolute_time()) - cycle_start;
    if (cycle_duration < constants::cycle_time) {
        // sleep_ms(constants::cycle_time - cycle_duration);
    }

    state::flight::cycle_count++;
    state::flight::timestamp = to_ms_since_boot(get_absolute_time());

    // Check for any cycle overflows
    if (state::flight::timestamp - cycle_start > constants::cycle_time) {
        state::flight::events.emplace_back(Event::cycle_overflow);
    }

    watchdog_update();

    logf("----------------END LOOP----------------\n\n");
}