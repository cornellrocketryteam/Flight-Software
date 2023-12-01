#include "fsw.hpp"


void Flight::execute() {
#ifdef DEBUG
    //sleep_ms(1000);
#endif
    
#ifdef VERBOSE
    printf("------------BEGIN LOOP------------\n");
    printf("Mode: %s\n", state::flight::mode->name().c_str());
    printf("Armed: %d\n\n", state::flight::armed);

    printf("Gyro X: %.3f\n", state::imu::gyro_x);
    printf("Gyro Y: %.3f\n", state::imu::gyro_y);
    printf("Gyro Z: %.3f\n\n", state::imu::gyro_z);

    printf("Mag X: %.3f\n", state::imu::mag_x);
    printf("Mag Y: %.3f\n", state::imu::mag_y);
    printf("Mag Z: %.3f\n\n", state::imu::mag_z);

    printf("Accel X: %.3f\n", state::accel::accel_x);
    printf("Accel Y: %.3f\n", state::accel::accel_y);
    printf("Accel Z: %.3f\n\n", state::accel::accel_z);

    printf("Temperature: %.3f\n", state::therm::temp);
    printf("Humidity: %.3f\n", state::therm::humidity);
#endif
    
    state::flight::mode->execute();
    state::flight::mode->transition();

#ifdef VERBOSE
    printf("-------------END LOOP-------------\n\n");
#endif
}