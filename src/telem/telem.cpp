/**
 * @file telem.cpp
 * @author csg83
 *
 * @brief Telemetry-related functionality
 */

#include "telem.hpp"
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
    metadata |= (static_cast<uint8_t>(state::voltage::status) & 0b1) << 6;
    metadata |= (static_cast<uint8_t>(state::therm::status) & 0b1) << 5;
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