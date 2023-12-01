#include "fsw.hpp"


void Flight::execute() {
#ifdef DEBUG
    sleep_ms(1000);
#endif
    
#ifdef VERBOSE
    printf("------------BEGIN LOOP------------\n");
    printf("Mode: %s\n", state::flight::mode->name().c_str());
    printf("Armed: %d\n", state::flight::armed);

    printf("Accel X: %.3f\n", state::accel::accel_x);
    printf("Accel Y: %.3f\n", state::accel::accel_y);
    printf("Accel Z: %.3f\n", state::accel::accel_z);

    printf("Temperature: %.3f\n", state::therm::temp);
    printf("Humidity: %.3f\n", state::therm::humidity);
#endif
    
    state::flight::mode->execute();
    state::flight::mode->transition();

#ifdef VERBOSE
    printf("-------------END LOOP-------------\n\n");
#endif
}