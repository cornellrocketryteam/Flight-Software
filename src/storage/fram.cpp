/**
 * @file fram.cpp
 * @author csg83
 *
 * @brief FRAM-related functionality
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

void FRAM::write(Data data) {
    switch (data) {
    case Data::boot_count:
        if (fram.write_bytes(static_cast<uint8_t>(Data::boot_count), reinterpret_cast<uint8_t *>(&state::flight::boot_count), 2)) {
            return;
        }
        break;
    case Data::flight_mode: {
        uint8_t mode = state::flight::mode->id();
        if (fram.write_bytes(static_cast<uint8_t>(Data::flight_mode), &mode, 1)) {
            return;
        }
        break;
    }
    case Data::alt_armed:
        if (fram.write_bytes(static_cast<uint8_t>(Data::alt_armed), reinterpret_cast<uint8_t *>(&state::flight::alt_armed), 1)) {
            return;
        }
        break;
    case Data::ref_pressure:
        if (fram.write_bytes(static_cast<uint8_t>(Data::ref_pressure), reinterpret_cast<uint8_t *>(&state::alt::ref_pressure), 4)) {
            return;
        }
        break;
    default:
        break;
    }
    state::flight::events.emplace_back(Event::fram_write_fail);
}