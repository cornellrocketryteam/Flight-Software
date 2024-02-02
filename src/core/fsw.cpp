#include "fsw.hpp"

void Flight::execute() {
#ifdef DEBUG
    // sleep_ms(1000);
#endif

#ifdef VERBOSE
    printf("---------------BEGIN LOOP %d---------------\n", state::flight::cycle_count);
    printf("Time: %d ms\n", state::flight::timestamp);
    printf("Mode: %s\n", state::flight::mode->name().c_str());
    printf("Altitude armed: %d\n\n", state::flight::altitude_armed);

    printf("ALTIMETER: %d\n", state::alt::status);
    printf("Altitude: %.3f m\n", state::alt::altitude);
    printf("Pressure: %.3f hPa\n\n", state::alt::pressure);

    printf("IMU: %d\n", state::imu::status);
    printf("Gyro X: %.3f m/s^2\n", state::imu::gyro_x);
    printf("Gyro Y: %.3f m/s^2\n", state::imu::gyro_y);
    printf("Gyro Z: %.3f m/s^2\n\n", state::imu::gyro_z);

    printf("Mag X: %.3f m/s^2\n", state::imu::mag_x);
    printf("Mag Y: %.3f m/s^2\n", state::imu::mag_y);
    printf("Mag Z: %.3f m/s^2\n\n", state::imu::mag_z);

    printf("ACCELEROMETER: %d\n", state::accel::status);
    printf("Accel X: %.3f m/s^2\n", state::accel::accel_x);
    printf("Accel Y: %.3f m/s^2\n", state::accel::accel_y);
    printf("Accel Z: %.3f m/s^2\n\n", state::accel::accel_z);

    printf("THERMOMETER: %d\n", state::therm::status);
    printf("Temperature: %.3f C\n", state::therm::temp);
    printf("Humidity: %.3f %%\n\n", state::therm::humidity);

    printf("SD: %d\n\n", state::sd::init);

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