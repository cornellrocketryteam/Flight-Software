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
        // printf("spi is writable AFTER BEGIN: %d\n", spi_is_writable(spi0));

        // spi_deinit(spi0);
        // spi_init(spi0, 100 * 1000);
        return false;
    }

    state = radio.setFrequency(900);
    if (state != RADIOLIB_ERR_NONE) {
#ifdef VERBOSE
        printf("Set Frequency failed, code %d\n", state);
#endif
        return false;
    }

    radio.setPacketSentAction(set_flag);

    return true;
}

bool RFM::transmit() {
    if (to_ms_since_boot(get_absolute_time()) > state::rfm::start_time + state::rfm::interrupt_delay) {
        state::rfm::start_time = UINT32_MAX - state::rfm::interrupt_delay;
        if (state == RADIOLIB_ERR_NONE) {
#ifdef VERBOSE
            printf("RFM: Transmit success\n");
#endif
        } else {
#ifdef VERBOSE
            printf("RFM Error: Transmit failed, code %d\n", state);
#endif
            return false;
        }

        radio.finishTransmit();

        uint8_t packet[constants::packet_size];

        uint16_t metadata = 0;

        metadata |= (state::flight::mode->id() & 0b111) << 13;

        metadata |= (static_cast<uint8_t>(state::alt::status) & 0b11) << 11;
        metadata |= (static_cast<uint8_t>(state::gps::status) & 0b11) << 9;
        metadata |= (static_cast<uint8_t>(state::imu::status) & 0b11) << 7;
        metadata |= (static_cast<uint8_t>(state::accel::status) & 0b11) << 5;
        metadata |= (static_cast<uint8_t>(state::therm::status) & 0b11) << 3;

        metadata |= (static_cast<uint8_t>(state::sd::init) & 0b1) << 2;
        metadata |= (static_cast<uint8_t>(state::gps::valid) & 0b1) << 1;
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

        memcpy(&packet[13], &state::gps::latitude, sizeof(int32_t));
        memcpy(&packet[17], &state::gps::longitude, sizeof(int32_t));
        memcpy(&packet[21], &state::gps::siv, sizeof(uint8_t));

        memcpy(&packet[22], &state::accel::accel_x, sizeof(float));
        memcpy(&packet[26], &state::accel::accel_y, sizeof(float));
        memcpy(&packet[30], &state::accel::accel_z, sizeof(float));

        memcpy(&packet[34], &state::imu::gyro_x, sizeof(float));
        memcpy(&packet[38], &state::imu::gyro_y, sizeof(float));
        memcpy(&packet[42], &state::imu::gyro_z, sizeof(float));

        memcpy(&packet[46], &state::imu::accel_x, sizeof(float));
        memcpy(&packet[50], &state::imu::accel_y, sizeof(float));
        memcpy(&packet[54], &state::imu::accel_z, sizeof(float));

        memcpy(&packet[58], &state::imu::orientation_x, sizeof(float));
        memcpy(&packet[62], &state::imu::orientation_y, sizeof(float));
        memcpy(&packet[66], &state::imu::orientation_z, sizeof(float));

        memcpy(&packet[70], &state::imu::gravity_x, sizeof(float));
        memcpy(&packet[74], &state::imu::gravity_y, sizeof(float));
        memcpy(&packet[78], &state::imu::gravity_z, sizeof(float));

        memcpy(&packet[82], &state::therm::temp, sizeof(float));

#ifdef VERBOSE
        printf("Transmitting: ");
        for(uint i=0; i<constants::packet_size; i++) {printf("%c", packet[i]);}
        printf("\n");
#endif

        state = radio.startTransmit(packet, constants::packet_size);
        return true;
    } else if (awaiting) {
        state::rfm::start_time = to_ms_since_boot(get_absolute_time());
        awaiting = false;
    }
    return false;
}