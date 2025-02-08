#include "fsw.hpp"

void Flight::execute() {

    logf("---------------BEGIN LOOP %d---------------\n", state::flight::cycle_count);
    logf("Time: %d ms\n", state::flight::timestamp);
    logf("Mode: %s\n", state::flight::mode->name().c_str());
    logf("Altitude armed: %d\n\n", state::flight::alt_armed);

    logf("ALTIMETER: %d\n", state::alt::status);
    logf("Altitude (m): %.3f\n", state::alt::altitude);
    logf("Pressure (hPa): %.3f\n\n", state::alt::pressure);

    logf("GPS: %d, Message: %d\n", state::gps::status, state::gps::data.valid);
    logf("Num Satellites: %d\n\n", state::gps::data.numSV);
    logf("Longitude: %d\n", state::gps::data.lon);
    logf("Latitude: %d\n", state::gps::data.lat);
    logf("Horizontal Acc. Est.: %d\n", state::gps::data.hAcc);
    logf("Vertical Acc. Est.: %d\n", state::gps::data.vAcc);
    logf("North Velocity: %d\n", state::gps::data.velN);
    logf("East Velocity: %d\n", state::gps::data.velE);
    logf("Down Velocity: %d\n", state::gps::data.velD);
    logf("Ground Speed: %d\n", state::gps::data.gSpeed);
    logf("Heading of Motion: %d\n", state::gps::data.headMot);
    logf("Speed Acc. Est: %d\n", state::gps::data.sAcc);
    logf("Heading Accuracy: %d\n", state::gps::data.headAcc);

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

    logf("BLiMS: %f\n\n", state::blims::motor_position);

    logf("SD: %d\n", state::sd::init);
    logf("RFM: %d\n\n", state::rfm::init);

    current_cycle_start = to_ms_since_boot(get_absolute_time());

    state::flight::mode->execute();
    state::flight::mode->transition();

    current_cycle_time = to_ms_since_boot(get_absolute_time()) - current_cycle_start;
    if (current_cycle_time < constants::cycle_time) {
        sleep_ms(constants::cycle_time - current_cycle_time);
    }

    logf("----------------END LOOP----------------\n\n");

    state::flight::cycle_count++;
    state::flight::timestamp = to_ms_since_boot(get_absolute_time());
}