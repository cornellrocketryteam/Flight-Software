#include "rfm.hpp"
#include "../constants.hpp"
#include "../core/state.hpp"
#include <bitset>

volatile bool awaiting = true;

void RFM::set_flag() {
    awaiting = true;
}

bool RFM::begin() {
    gpio_init(RFM_CS);
    gpio_init(RFM_RST);

    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_set_dir(RFM_RST, GPIO_OUT);

    sleep_ms(10);
    gpio_put(RFM_RST, 0);
    sleep_ms(10);
    gpio_put(RFM_RST, 1);

    state = radio.begin();
    if (state != RADIOLIB_ERR_NONE) {
#ifdef VERBOSE
        printf("RFM Error: Init failed, code %d\n", state);
#endif
        return false;
    }

    radio.setPacketSentAction(set_flag);

    return true;
}

bool RFM::transmit() {
    if (awaiting) {
        awaiting = false;

        if (state == RADIOLIB_ERR_NONE) {
#ifdef VERBOSE
            printf("RFM: Transmit success\n");
#endif
        } else {
#ifdef VERBOSE
            printf("RFM Error: Transmit failed, code %d\n", state);
            return false;
        }
#endif
        radio.finishTransmit();

        uint8_t packet[61];

        uint16_t metadata = 0;

        metadata |= (state::flight::mode->id() & 0x07) << 13;

        metadata |= (static_cast<uint8_t>(state::alt::status) & 0x03) << 11;
        metadata |= (static_cast<uint8_t>(state::gps::status) & 0x03) << 9;
        metadata |= (static_cast<uint8_t>(state::imu::status) & 0b11) << 7;
        metadata |= (static_cast<uint8_t>(state::accel::status) & 0b11) << 5;
        metadata |= (static_cast<uint8_t>(state::therm::status) & 0b11) << 3;

        metadata |= (static_cast<uint8_t>(state::sd::init) & 0b1) << 2;
        metadata |= (static_cast<uint8_t>(state::flight::key_armed) & 0b1) << 1;
        metadata |= (static_cast<uint8_t>(state::flight::alt_armed) & 0b1);

        std::bitset<24> event_bits;
        for (Event event : state::flight::events) {
            event_bits.set(static_cast<uint8_t>(event));
        }

        memcpy(&packet[0], &metadata, sizeof(uint16_t));
        memcpy(&packet[2], &state::flight::timestamp, sizeof(uint32_t));

        packet[6] = static_cast<uint8_t>(event_bits.to_ulong() & 0xFF);
        packet[7] = static_cast<uint8_t>((event_bits.to_ulong() >> 8) & 0xFF);
        packet[8] = static_cast<uint8_t>((event_bits.to_ulong() >> 8) & 0xFF);

        memcpy(&packet[9], &state::alt::altitude, sizeof(float));

        memcpy(&packet[13], &state::gps::latitude, sizeof(float));
        memcpy(&packet[17], &state::gps::longitude, sizeof(float));
        memcpy(&packet[21], &state::gps::altitude, sizeof(float));

        memcpy(&packet[25], &state::accel::accel_x, sizeof(float));
        memcpy(&packet[29], &state::accel::accel_y, sizeof(float));
        memcpy(&packet[33], &state::accel::accel_z, sizeof(float));

        memcpy(&packet[37], &state::imu::gyro_x, sizeof(float));
        memcpy(&packet[41], &state::imu::gyro_y, sizeof(float));
        memcpy(&packet[45], &state::imu::gyro_z, sizeof(float));

        memcpy(&packet[49], &state::imu::mag_x, sizeof(float));
        memcpy(&packet[53], &state::imu::mag_y, sizeof(float));
        memcpy(&packet[57], &state::imu::mag_z, sizeof(float));

        state = radio.startTransmit(packet, 61);
        return true;
    }
    return false;
}