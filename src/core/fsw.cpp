#include "fsw.hpp"

void Flight::execute() {

#ifdef VERBOSE
    printf("---------------BEGIN LOOP %d---------------\n", state::flight::cycle_count);
    printf("Time: %d ms\n", state::flight::timestamp);
    printf("Mode: %s\n", state::flight::mode->name().c_str());
    printf("Altitude armed: %d\n\n", state::flight::altitude_armed);

    printf("ALTIMETER: %d\n", state::alt::status);
    printf("Altitude (m): %.3f\n", state::alt::altitude);
    printf("Pressure (hPa): %.3f\n\n", state::alt::pressure);

    printf("IMU: %d\n", state::imu::status);
    printf("Gyro X (m/s^2): %.3f\n", state::imu::gyro_x);
    printf("Gyro Y (m/s^2): %.3f\n", state::imu::gyro_y);
    printf("Gyro Z (m/s^2): %.3f\n\n", state::imu::gyro_z);

    printf("Mag X (m/s^2): %.3f\n", state::imu::mag_x);
    printf("Mag Y (m/s^2): %.3f\n", state::imu::mag_y);
    printf("Mag Z (m/s^2): %.3f\n\n", state::imu::mag_z);

    printf("ACCELEROMETER: %d\n", state::accel::status);
    printf("Accel X (m/s^2): %.3f\n", state::accel::accel_x);
    printf("Accel Y (m/s^2): %.3f\n", state::accel::accel_y);
    printf("Accel Z (m/s^2): %.3f\n\n", state::accel::accel_z);

    printf("THERMOMETER: %d\n", state::therm::status);
    printf("Temperature (C): %.3f\n\n", state::therm::temp);

    printf("SD: %d\n", state::sd::init);
    printf("RFM: %d\n\n", state::rfm::init);

#endif

    state::flight::mode->execute();
    state::flight::mode->transition();

#ifdef VERBOSE
    printf("----------------END LOOP----------------\n\n");
#endif
    state::flight::cycle_count++;
    state::flight::timestamp = to_ms_since_boot(get_absolute_time());
}