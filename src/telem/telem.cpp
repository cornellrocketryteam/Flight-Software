/**
 * @file telem.cpp
 * @author csg83
 *
 * @brief Telemetry-related functionality
 */

#include "telem.hpp"
#include "pins.hpp"
#include "state.hpp"
#include <bitset>

void Telem::pack_data() {
    packed_metadata = 0;
    packed_metadata |= (state::flight::mode->id() & 0b111) << 13;

    packed_metadata |= (static_cast<uint8_t>(state::actuator::sv_open) & 0b1) << 12;
    packed_metadata |= (static_cast<uint8_t>(state::actuator::mav_open) & 0b1) << 11;
    packed_metadata |= (static_cast<uint8_t>(state::flight::safed) & 0b1) << 10;
    packed_metadata |= (static_cast<uint8_t>(state::gps::valid) & 0b1) << 9;
    packed_metadata |= (static_cast<uint8_t>(state::sd::init) & 0b1) << 8;
    packed_metadata |= (static_cast<uint8_t>(state::fram::init) & 0b1) << 7;
    packed_metadata |= (static_cast<uint8_t>(state::adc::status) & 0b1) << 6;
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
    memcpy(&packet[18], &state::alt::temp, sizeof(float));

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

    memcpy(&packet[103], &state::blims::motor_position, sizeof(float));

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

    for (uint i = 0; i < constants::umb_packet_size; ++i) {
        printf("%c", packet[i]);
    }
    printf("\n");
}