/**
 * @file telem.cpp
 * @author csg83
 *
 * @brief Telemetry-related functionality
 */

#include "telem.hpp"
#include "hardware/watchdog.h"
#include "modules.hpp"
#include "pins.hpp"
#include "state.hpp"
#include "tusb.h"

void Telem::pack_data() {
    packed_metadata = 0;
    packed_metadata |= (state::flight::mode->id() & 0b111) << 13;

    packed_metadata |= (static_cast<uint8_t>(state::actuator::sv_open) & 0b1) << 12;
    packed_metadata |= (static_cast<uint8_t>(state::actuator::mav_open) & 0b1) << 11;
    packed_metadata |= (static_cast<uint8_t>(state::flight::safed) & 0b1) << 10;
    packed_metadata |= (static_cast<uint8_t>(state::gps::fresh) & 0b1) << 9;
    packed_metadata |= (static_cast<uint8_t>(state::sd::init) & 0b1) << 8;
    packed_metadata |= (static_cast<uint8_t>(state::fram::init) & 0b1) << 7;
    packed_metadata |= (static_cast<uint8_t>(state::adc::status) & 0b1) << 6;
    packed_metadata |= (static_cast<uint8_t>(state::umb::connected) & 0b1) << 5;
    packed_metadata |= (static_cast<uint8_t>(state::accel::status) & 0b1) << 4;
    packed_metadata |= (static_cast<uint8_t>(state::imu::status) & 0b1) << 3;
    packed_metadata |= (static_cast<uint8_t>(state::gps::status) & 0b1) << 2;
    packed_metadata |= (static_cast<uint8_t>(state::alt::status) & 0b1) << 1;
    packed_metadata |= (static_cast<uint8_t>(state::flight::alt_armed) & 0b1);

    packed_events = events.get();
}

void RFM::transmit() {
    pack_data();

    memcpy(&packet[0], &sync_word, sizeof(uint32_t));
    memcpy(&packet[4], &packed_metadata, sizeof(uint16_t));
    memcpy(&packet[6], &state::flight::timestamp, sizeof(uint32_t));
    memcpy(&packet[10], &events, sizeof(uint32_t));

    memcpy(&packet[14], &state::alt::altitude, sizeof(float));
    memcpy(&packet[18], &state::alt::temperature, sizeof(float));

    memcpy(&packet[22], &state::gps::data.lat, sizeof(int32_t));
    memcpy(&packet[26], &state::gps::data.lon, sizeof(int32_t));
    memcpy(&packet[30], &state::gps::data.numSV, sizeof(uint8_t));
    memcpy(&packet[31], &state::gps::unix_time, sizeof(uint32_t));
    memcpy(&packet[35], &state::gps::data.hAcc, sizeof(uint32_t));

    memcpy(&packet[39], &state::imu::accel_x, sizeof(float));
    memcpy(&packet[43], &state::imu::accel_y, sizeof(float));
    memcpy(&packet[47], &state::imu::accel_z, sizeof(float));
    memcpy(&packet[51], &state::imu::gyro_x, sizeof(float));
    memcpy(&packet[55], &state::imu::gyro_y, sizeof(float));
    memcpy(&packet[59], &state::imu::gyro_z, sizeof(float));
    memcpy(&packet[63], &state::imu::orientation_x, sizeof(float));
    memcpy(&packet[67], &state::imu::orientation_y, sizeof(float));
    memcpy(&packet[71], &state::imu::orientation_z, sizeof(float));

    memcpy(&packet[75], &state::accel::accel_x, sizeof(float));
    memcpy(&packet[79], &state::accel::accel_y, sizeof(float));
    memcpy(&packet[83], &state::accel::accel_z, sizeof(float));

    memcpy(&packet[87], &state::adc::battery_voltage, sizeof(float));
    memcpy(&packet[91], &state::adc::pressure_pt3, sizeof(float));
    memcpy(&packet[95], &state::adc::pressure_pt4, sizeof(float));
    memcpy(&packet[99], &state::adc::temp_rtd, sizeof(float));

#ifdef USE_BLIMS
    memcpy(&packet[103], &state::blims::motor_position, sizeof(float));
#endif

    uart_write_blocking(UART_PORT, (const uint8_t *)packet, constants::rfm_packet_size);
}

void Umbilical::transmit() {
    pack_data();

    memcpy(&packet[0], &packed_metadata, sizeof(uint16_t));
    memcpy(&packet[2], &state::flight::timestamp, sizeof(uint32_t));
    memcpy(&packet[6], &events, sizeof(uint32_t));

    memcpy(&packet[10], &state::adc::battery_voltage, sizeof(float));
    memcpy(&packet[14], &state::adc::pressure_pt3, sizeof(float));
    memcpy(&packet[18], &state::adc::pressure_pt4, sizeof(float));
    memcpy(&packet[22], &state::adc::temp_rtd, sizeof(float));

    tud_cdc_write(packet, constants::umb_packet_size);
    tud_cdc_write("\n", 1);
    tud_cdc_write_flush();
}

void Umbilical::check_command() {
    int c = getchar_timeout_us(0);

    while (c != PICO_ERROR_TIMEOUT) {
        if (!receiving) {
            if (c == constants::command_start) {
                receiving = true;
                command_index = 0;
            }
        } else if (c == constants::command_stop) {
            receiving = false;
            command_buffer[command_index] = '\0';
            command_index = 0;
            process_command();
        } else {
            if (command_index < sizeof(command_buffer) - 1) {
                command_buffer[command_index++] = c;
            } else {
                receiving = false;
                command_index = 0;
                events.push(Event::unknown_command_received);
            }
        }
        c = getchar_timeout_us(0);
    }
}

void Umbilical::process_command() {
    if (strcmp(command_buffer, command::launch) == 0) {
        if (state::flight::mode->id() != 1) {
            return;
        }
        state::umb::launch_commanded = true;
        events.push(Event::launch_command_received);

    } else if (strcmp(command_buffer, command::mav_open) == 0) {
        mav.open();
        events.push(Event::mav_command_received);

    } else if (strcmp(command_buffer, command::mav_close) == 0) {
        mav.close();
        events.push(Event::mav_command_received);

    } else if (strcmp(command_buffer, command::sv_open) == 0) {
        sv.open();
        events.push(Event::sv_command_received);

    } else if (strcmp(command_buffer, command::sv_close) == 0) {
        sv.close();
        events.push(Event::sv_command_received);

    } else if (strcmp(command_buffer, command::safe) == 0) {
        sv.open();
        state::flight::safed = true;
        events.push(Event::safe_command_received);

    } else if (strcmp(command_buffer, command::reset_card) == 0) {
        sd.reset_data();
        events.push(Event::reset_card_command_received);

    } else if (strcmp(command_buffer, command::reset_fram) == 0) {
        fram.reset_data();
        events.push(Event::reset_fram_command_received);

    } else if (strcmp(command_buffer, command::reboot) == 0) {
        watchdog_reboot(0, 0, 0);

    } else if (strncmp(command_buffer, command::change_target_lat, 2) == 0) {
#ifdef USE_BLIMS
        state::blims::target_lat = atof(command_buffer + 2);
        fram.store(Data::blims_target_lat);
#endif
        events.push(Event::state_change_command_received);

    } else if (strncmp(command_buffer, command::change_target_long, 2) == 0) {
#ifdef USE_BLIMS
        state::blims::target_long = atof(command_buffer + 2);
        fram.store(Data::blims_target_long);
#endif
        events.push(Event::state_change_command_received);

    } else if (strncmp(command_buffer, command::change_ref_pressure, 2) == 0) {
        state::alt::ref_pressure = atof(command_buffer + 2);
        events.push(Event::state_change_command_received);

    } else if (strncmp(command_buffer, command::change_alt_state, 2) == 0) {
        char state = command_buffer[2];
        if (state == '0' || state == '1') {
            state::alt::status = static_cast<SensorState>(state - '0');
            events.push(Event::state_change_command_received);
        } else {
            events.push(Event::unknown_command_received);
        }

    } else if (strncmp(command_buffer, command::change_card_state, 2) == 0) {
        char state = command_buffer[2];
        if (state == '0' || state == '1') {
            state::sd::init = (state == '1');
            events.push(Event::state_change_command_received);
        } else {
            events.push(Event::unknown_command_received);
        }

    } else if (strncmp(command_buffer, command::change_alt_armed, 2) == 0) {
        char state = command_buffer[2];
        if (state == '0' || state == '1') {
            state::flight::alt_armed = (state == '1');
            events.push(Event::state_change_command_received);
        } else {
            events.push(Event::unknown_command_received);
        }

    } else if (strncmp(command_buffer, command::change_flight_mode, 2) == 0) {
        int state = command_buffer[2] - '0';
        if (0 <= state && state <= 5) {
            switch (state) {
            case 0:
                state::flight::mode = state::flight::startup;
                break;
            case 1:
                state::flight::mode = state::flight::standby;
                break;
            case 2:
                state::flight::mode = state::flight::ascent;
                break;
            case 3:
                state::flight::mode = state::flight::drogue_deployed;
                break;
            case 4:
                state::flight::mode = state::flight::main_deployed;
                break;
            case 5:
                state::flight::mode = state::flight::fault;
                break;
            }
            if (state::fram::init) {
                fram.store(Data::old_mode);
            }
            events.push(Event::state_change_command_received);
        } else {
            events.push(Event::unknown_command_received);
        }

    } else {
        events.push(Event::unknown_command_received);
    }
}

bool Umbilical::connection_changed() {
    if (state::umb::connected) {
        if (!tud_cdc_connected()) {
            failed_connections++;
        } else {
            failed_connections = 0;
        }
        if (failed_connections == constants::max_umb_failed_reads) {
            state::umb::connected = false;
            failed_connections = 0;
            return true;
        }
    } else {
        if (tud_cdc_connected()) {
            successful_connections++;
        } else {
            successful_connections = 0;
        }
        if (successful_connections == constants::min_umb_successful_reads) {
            state::umb::connected = true;
            successful_connections = 0;
            return true;
        }
    }
    return false;
}