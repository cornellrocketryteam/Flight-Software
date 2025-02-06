/**
 * @file fsw.cpp
 * @author csg83
 *
 * @brief The main flight loop
 */

#include "fsw.hpp"
#include "hardware/watchdog.h"

void Flight::execute() {

    logf("----------------BEGIN LOOP %d----------------\n", state::flight::cycle_count);
    logf("Time: %d ms\n", state::flight::timestamp);
    logf("Mode: %s           ", state::flight::mode->name().c_str());
    logf("Altitude armed: %d\n\n", state::flight::alt_armed);

    logf("ALTIMETER: %d ---------- ", state::alt::status);
    logf("Altitude (m): %.3f\n%s", state::alt::altitude, SPACER);
    logf("Temperature (C): %.3f\n\n", state::alt::temp);

    logf("GPS: %d, Message: %d ---- ", state::gps::status, state::gps::data.valid);
    logf("Latitude: %f\n%s", state::gps::data.lat, SPACER);
    logf("Longitude: %f\n%s", state::gps::data.lon, SPACER);
    logf("Num Satellites: %d\n\n", state::gps::data.numSV);

    logf("IMU: %d ---------------- ", state::imu::status);
    logf("Gyro X (deg/s): %.3f\n%s", state::imu::gyro_x, SPACER);
    logf("Gyro Y (deg/s): %.3f\n%s", state::imu::gyro_y, SPACER);
    logf("Gyro Z (deg/s): %.3f\n\n%s", state::imu::gyro_z, SPACER);

    logf("Accel X (m/s^2): %.3f\n%s", state::imu::accel_x, SPACER);
    logf("Accel Y (m/s^2): %.3f\n%s", state::imu::accel_y, SPACER);
    logf("Accel Z (m/s^2): %.3f\n\n%s", state::imu::accel_z, SPACER);

    logf("Orientation X (deg): %.3f\n%s", state::imu::orientation_x, SPACER);
    logf("Orientation Y (deg): %.3f\n%s", state::imu::orientation_y, SPACER);
    logf("Orientation Z (deg): %.3f\n\n%s", state::imu::orientation_z, SPACER);

    logf("Gravity X (m/s^2): %.3f\n%s", state::imu::gravity_x, SPACER);
    logf("Gravity Y (m/s^2): %.3f\n%s", state::imu::gravity_y, SPACER);
    logf("Gravity Z (m/s^2): %.3f\n\n", state::imu::gravity_z);

    logf("ACCELEROMETER: %d ------ ", state::accel::status);
    logf("Accel X (G): %.3f\n%s", state::accel::accel_x, SPACER);
    logf("Accel Y (G): %.3f\n%s", state::accel::accel_y, SPACER);
    logf("Accel Z (G): %.3f\n\n", state::accel::accel_z);

    logf("ADC: %d ---------------- ", state::adc::status);
    logf("PT 3 (PSI): %.3f\n%s", state::adc::pressure_pt3, SPACER);
    logf("PT 4 (PSI): %.3f\n%s", state::adc::pressure_pt4, SPACER);
    logf("RTD (C): %.3f\n\n", state::adc::temp_rtd);

    logf("SD: %d ----------------- ", state::sd::init);
    logf("Current File: %d\n\n", state::sd::current_file);

    logf("FRAM: %d --------------- ", state::fram::init);
    logf("Boot Count: %d\n%s", state::flight::boot_count, SPACER);
    logf("Watchdog Boot Count: %d\n%s", state::flight::watchdog_boot_count, SPACER);
    logf("Old Mode: %d\n\n", state::flight::old_mode);

    logf("MAV: %d%s", state::mav::open, "                  ");
    logf("SV: %d\n\n", state::sv::open);

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
}