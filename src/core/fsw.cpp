#include "fsw.hpp"


void Flight::execute() {
#ifdef DEBUG
    sleep_ms(3000);
#endif
    
#ifdef VERBOSE
    printf("------------BEGIN LOOP------------\n");
    printf("Mode: %s\n", state::flight::mode->name().c_str());
    printf("Armed: %d\n", state::flight::armed);
#endif
    
    state::flight::mode->execute();
    state::flight::mode->transition();

#ifdef VERBOSE
    printf("-------------END LOOP-------------\n\n");
#endif
}