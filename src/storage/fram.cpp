/**
 * @file fram.cpp
 * @author csg83
 *
 * @brief FRAM-related functionality
 */

#include "fram.hpp"
#include "events.hpp"
#include "hardware/watchdog.h"
#include "modules.hpp"
#include "pins.hpp"
#include "state.hpp"

FRAM::FRAM() : fram(SPI_PORT, FRAM_CS) {}

void FRAM::begin() {
    if (fram.begin()) {
        state::fram::init = true;
        load(Data::boot_count);
        load(Data::old_mode);
        load(Data::watchdog_boot_count);
        load(Data::mav_state);
        load(Data::sv_state);

        state::flight::boot_count++;
        store(Data::boot_count);

        if (watchdog_caused_reboot()) {
            state::flight::watchdog_boot_count++;
        }
        store(Data::watchdog_boot_count);

        // Load old reference pressure if we're in flight
        if (state::flight::old_mode > 1) {
            load(Data::ref_pressure);
        }

        // Set SV state from previous boot
        if (!state::actuator::sv_open) {
            sv.close();
        }
    } else {
        events.push(Event::fram_init_fail);
    }
}

void FRAM::load(Data data) {
    switch (data) {
    case Data::boot_count: {
        uint8_t boot_count[2];
        if (fram.read_bytes(static_cast<uint8_t>(Data::boot_count), boot_count, 2)) {
            state::flight::boot_count = (boot_count[1] << 8) | boot_count[0];
            return;
        }
        break;
    }
    case Data::old_mode: {
        uint8_t old_mode;
        if (fram.read_bytes(static_cast<uint8_t>(Data::old_mode), &old_mode, 1)) {
            state::flight::old_mode = old_mode;
            return;
        }
        break;
    }
    case Data::watchdog_boot_count: {
        uint8_t watchdog_boot_count;
        if (fram.read_bytes(static_cast<uint8_t>(Data::watchdog_boot_count), &watchdog_boot_count, 1)) {
            state::flight::watchdog_boot_count = watchdog_boot_count;
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
    case Data::mav_state: {
        uint8_t mav_state;
        if (fram.read_bytes(static_cast<uint8_t>(Data::mav_state), &mav_state, 1)) {
            state::actuator::mav_open = (bool)mav_state;
            return;
        }
        break;
    }
    case Data::sv_state: {
        uint8_t sv_state;
        if (fram.read_bytes(static_cast<uint8_t>(Data::sv_state), &sv_state, 1)) {
            state::actuator::sv_open = (bool)sv_state;
            return;
        }
        break;
    }
    case Data::pt_index: {
        uint8_t pt_index[2];
        if (fram.read_bytes(static_cast<uint8_t>(Data::pt_index), pt_index, 2)) {
            state::fram::pt_index = (pt_index[1] << 8) | pt_index[0];
            return;
        }
    }
    default:
        break;
    }
    events.push(Event::fram_read_fail);
}

void FRAM::store(Data data) {
    switch (data) {
    case Data::boot_count:
        if (fram.write_bytes(static_cast<uint8_t>(Data::boot_count), reinterpret_cast<uint8_t *>(&state::flight::boot_count), 2)) {
            return;
        }
        break;
    case Data::old_mode: {
        uint8_t mode = state::flight::mode->id();
        if (fram.write_bytes(static_cast<uint8_t>(Data::old_mode), &mode, 1)) {
            return;
        }
        break;
    }
    case Data::watchdog_boot_count:
        if (fram.write_bytes(static_cast<uint8_t>(Data::watchdog_boot_count), &state::flight::watchdog_boot_count, 1)) {
            return;
        }
        break;
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
    case Data::mav_state:
        if (fram.write_bytes(static_cast<uint8_t>(Data::mav_state), reinterpret_cast<uint8_t *>(&state::actuator::mav_open), 1)) {
            return;
        }
        break;
    case Data::sv_state:
        if (fram.write_bytes(static_cast<uint8_t>(Data::sv_state), reinterpret_cast<uint8_t *>(&state::actuator::sv_open), 1)) {
            return;
        }
        break;
    case Data::pt: {
        // Write PT3 and PT4 values
        if (!fram.write_bytes(state::fram::pt_index, reinterpret_cast<uint8_t *>(&state::adc::pressure_pt3), 4)) {
            events.push(Event::fram_write_fail);
        }
        if (!fram.write_bytes(state::fram::pt_index + sizeof(float), reinterpret_cast<uint8_t *>(&state::adc::pressure_pt4), 4)) {
            events.push(Event::fram_write_fail);
        }

        // Update PT index in both state and FRAM
        state::fram::pt_index += 2 * sizeof(float);
        if (!fram.write_bytes(static_cast<uint8_t>(Data::pt_index), reinterpret_cast<uint8_t *>(&state::fram::pt_index), 2)) {
            events.push(Event::fram_write_fail);
        }

        return;
    }
    default:
        break;
    }
    events.push(Event::fram_write_fail);
}

void FRAM::reset_data() {
    if (!fram.write_bytes(0, clear, sizeof(clear))) {
        events.push(Event::fram_write_fail);
        return;
    }

    fram.write_bytes(static_cast<uint8_t>(Data::sv_state), reinterpret_cast<uint8_t *>(&state::actuator::sv_open), 1);
}