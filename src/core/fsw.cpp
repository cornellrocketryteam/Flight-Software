#include "fsw.hpp"


void Flight::execute() {
    state::flight::mode->execute();
    state::flight::mode->transition();
}