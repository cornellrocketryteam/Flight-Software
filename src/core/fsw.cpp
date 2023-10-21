#include "fsw.hpp"


void Flight::execute() {
    // sleep_ms(5000);
    state::flight::mode->execute();
    state::flight::mode->transition();
}