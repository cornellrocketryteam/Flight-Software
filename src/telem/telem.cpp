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
    metadata = 0;
    metadata |= (state::flight::mode->id() & 0b111) << 13;

    metadata |= (static_cast<uint8_t>(state::sv::open) & 0b1) << 12;
    metadata |= (static_cast<uint8_t>(state::mav::open) & 0b1) << 11;

    metadata |= (static_cast<uint8_t>(state::gps::valid) & 0b1) << 10;
    metadata |= (static_cast<uint8_t>(state::sd::init) & 0b1) << 9;
    metadata |= (static_cast<uint8_t>(state::fram::init) & 0b1) << 8;
    metadata |= (static_cast<uint8_t>(state::adc::status) & 0b1) << 7;
    metadata |= (static_cast<uint8_t>(state::accel::status) & 0b1) << 4;
    metadata |= (static_cast<uint8_t>(state::imu::status) & 0b1) << 3;
    metadata |= (static_cast<uint8_t>(state::gps::status) & 0b1) << 2;
    metadata |= (static_cast<uint8_t>(state::alt::status) & 0b1) << 1;
    metadata |= (static_cast<uint8_t>(state::flight::alt_armed) & 0b1);

    for (Event event : state::flight::events) {
        events |= (1 << static_cast<uint8_t>(event));
    }
}

void RFM::transmit() {
    pack_data();

    memcpy(&packet[0], &metadata, sizeof(uint16_t));
    memcpy(&packet[2], &state::flight::timestamp, sizeof(uint32_t));
    memcpy(&packet[6], &events, sizeof(uint32_t));

    memcpy(&packet[10], &state::alt::altitude, sizeof(float));
    memcpy(&packet[14], &state::gps::data.latitude, sizeof(int32_t));
    memcpy(&packet[18], &state::gps::data.longitude, sizeof(int32_t));
    memcpy(&packet[22], &state::gps::data.utc_time, sizeof(uint8_t));
    memcpy(&packet[23], &state::gps::data.num_satellites, sizeof(uint32_t));
    memcpy(&packet[27], &state::imu::accel_x, sizeof(float));
    memcpy(&packet[31], &state::imu::accel_y, sizeof(float));
    memcpy(&packet[35], &state::imu::accel_z, sizeof(float));
    memcpy(&packet[39], &state::imu::gyro_x, sizeof(float));
    memcpy(&packet[43], &state::imu::gyro_y, sizeof(float));
    memcpy(&packet[47], &state::imu::gyro_z, sizeof(float));
    memcpy(&packet[51], &state::imu::orientation_x, sizeof(float));
    memcpy(&packet[55], &state::imu::orientation_y, sizeof(float));
    memcpy(&packet[59], &state::imu::orientation_z, sizeof(float));
    memcpy(&packet[63], &state::imu::gravity_x, sizeof(float));
    memcpy(&packet[67], &state::imu::gravity_y, sizeof(float));
    memcpy(&packet[71], &state::imu::gravity_z, sizeof(float));
    memcpy(&packet[75], &state::accel::accel_x, sizeof(float));
    memcpy(&packet[79], &state::accel::accel_y, sizeof(float));
    memcpy(&packet[83], &state::accel::accel_z, sizeof(float));
    memcpy(&packet[87], &state::alt::temp, sizeof(float));
    memcpy(&packet[91], &state::adc::pressure_pt3, sizeof(float));
    memcpy(&packet[95], &state::adc::pressure_pt4, sizeof(float));
    memcpy(&packet[99], &state::blims::motor_position, sizeof(float));

    uart_write_blocking(UART_PORT, (const uint8_t *)packet, constants::rfm_packet_size);
}

void Umbilical::transmit() {
    pack_data();

    memcpy(&packet[0], &metadata, sizeof(uint16_t));
    memcpy(&packet[2], &state::flight::timestamp, sizeof(uint32_t));
    memcpy(&packet[6], &events, sizeof(uint32_t));

    // TODO: Probably remove
    memcpy(&packet[10], &state::rfm::init, sizeof(bool));
    memcpy(&packet[11], &state::rfm::init, sizeof(bool));

    for (uint i = 0; i < constants::umb_packet_size; ++i) {
        printf("%s ", std::bitset<8>(static_cast<uint8_t>(packet[i])).to_string().c_str());
    }
    printf("\n");
}