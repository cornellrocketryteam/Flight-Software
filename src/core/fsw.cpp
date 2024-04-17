#include "fsw.hpp"

void Flight::execute() {

// #ifdef VERBOSE
    printf("---------------BEGIN LOOP %d---------------\n", state::flight::cycle_count);
    printf("Time: %d ms\n", state::flight::timestamp);
    printf("Mode: %s\n", state::flight::mode->name().c_str());
    printf("Altitude armed: %d\n\n", state::flight::alt_armed);

    printf("ALTIMETER: %d\n", state::alt::status);
    printf("Altitude (m): %.3f\n", state::alt::altitude);
    printf("Pressure (hPa): %.3f\n\n", state::alt::pressure);

    printf("IMU: %d\n", state::imu::status);
    printf("Gyro X (deg/s): %.3f\n", state::imu::gyro_x);
    printf("Gyro Y (deg/s): %.3f\n", state::imu::gyro_y);
    printf("Gyro Z (deg/s): %.3f\n\n", state::imu::gyro_z);

    printf("Accel X (m/s^2): %.3f\n", state::imu::accel_x);
    printf("Accel Y (m/s^2): %.3f\n", state::imu::accel_y);
    printf("Accel Z (m/s^2): %.3f\n\n", state::imu::accel_z);

    printf("Orientation X (deg): %.3f\n", state::imu::orientation_x);
    printf("Orientation Y (deg): %.3f\n", state::imu::orientation_y);
    printf("Orientation Z (deg): %.3f\n\n", state::imu::orientation_z);

    printf("Gravity X (m/s^2): %.3f\n", state::imu::gravity_x);
    printf("Gravity Y (m/s^2): %.3f\n", state::imu::gravity_y);
    printf("Gravity Z (m/s^2): %.3f\n\n", state::imu::gravity_z);

    printf("ACCELEROMETER: %d\n", state::accel::status);
    printf("Accel X (G): %.3f\n", state::accel::accel_x);
    printf("Accel Y (G): %.3f\n", state::accel::accel_y);
    printf("Accel Z (G): %.3f\n\n", state::accel::accel_z);

    printf("THERMOMETER: %d\n", state::therm::status);
    printf("Temperature (C): %.3f\n\n", state::therm::temp);

    printf("SD: %d\n", state::sd::init);
    printf("RFM: %d\n\n", state::rfm::init);

// #endif

    state::flight::mode->execute();
    state::flight::mode->transition();

// #ifdef VERBOSE
    printf("----------------END LOOP----------------\n\n");
// #endif
    state::flight::cycle_count++;
    state::flight::timestamp = to_ms_since_boot(get_absolute_time());
}