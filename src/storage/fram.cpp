/**
 * @file fram.cpp
 * @author csg83
 *
 * @brief FRAM-related definitions
 */

#include "fram.hpp"
#include "pins.hpp"
#include "state.hpp"

FRAM::FRAM() : fram(SPI_PORT, FRAM_CS) {}

bool FRAM::begin() {
    if (fram.begin()) {
        state::fram::init = true;
        return true;
    } else {
        state::flight::events.emplace_back(Event::fram_init_fail);
        return false;
    }
}

void FRAM::write_mode() {
}