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
        load(Data::boot_count);
        load(Data::flight_mode);
        // TODO: Load and process other data
        return true;
    } else {
        state::flight::events.emplace_back(Event::fram_init_fail);
        return false;
    }
}

void FRAM::load(Data data) {
    switch (data) {
    case Data::boot_count: {
        uint8_t boot_count[2];
        if (fram.read_bytes(static_cast<uint8_t>(Data::boot_count), boot_count, 2)) {
            state::flight::boot_count = (boot_count[1] << 8) | boot_count[0];
            state::flight::boot_count++;
            return;
        }
        break;
    }
    case Data::flight_mode: {
        uint8_t old_mode;
        if (fram.read_bytes(static_cast<uint8_t>(Data::flight_mode), &old_mode, 1)) {
            state::flight::old_mode = old_mode;
            return;
        }
        break;
    }
    case Data::alt_armed: {
        uint8_t alt_armed;
        if (fram.read_bytes(static_cast<uint8_t>(Data::alt_armed), &alt_armed, 1)) {
            state::flight::alt_armed = (bool)alt_armed;
            return;
        }
        break;
    }
    case Data::ref_pressure: {
        uint8_t ref_pressure[4];
        if (fram.read_bytes(static_cast<uint8_t>(Data::ref_pressure), ref_pressure, 4)) {
            state::alt::ref_pressure = (ref_pressure[3] << 24) | (ref_pressure[2] << 16) | (ref_pressure[1] << 8) | ref_pressure[0];
            return;
        }
        break;
    }
    default:
        break;
    }
    state::flight::events.emplace_back(Event::fram_read_fail);
}

void FRAM::store(Data data) {
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