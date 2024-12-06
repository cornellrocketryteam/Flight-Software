#include "ublox_mx.hpp"
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

GNSS::GNSS(i2c_inst_t *i2c_type) {
    i2c = i2c_type;
}

bool GNSS::begin() {
    return true;
}

// TODO: Abstract to any NMEA message
bool GNSS::parse_gngga(gnss_data_t *data) {
    size_t start = raw_data.find("$GNGGA");
    if (start == std::string::npos) {
        return false;
    }

    size_t end = raw_data.find('\n', start);
    if (end == std::string::npos) {
        end = raw_data.length();
    }

    std::string sentence = raw_data.substr(start, end - start);

    std::istringstream ss(sentence);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, ',')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 8) {
#ifdef VERBOSE
        printf("Error: Incomplete GNGGA message\n");
#endif
        return false;
    }

    // Latitude
    if (!tokens[2].empty() && !tokens[3].empty()) {
        data->latitude = tokens[2];
    } else {
#ifdef VERBOSE
        printf("Error: Missing latitude data in GNGGA message\n");
#endif
        return false;
    }

    // Longitude
    if (!tokens[4].empty() && !tokens[5].empty()) {
        data->longitude = tokens[4];
    } else {
#ifdef VERBOSE
        printf("Error: Missing longitude data in GNGGA message\n");
#endif
        return false;
    }

    // Number of Satellites
    if (!tokens[7].empty()) {
        data->num_satellites = tokens[7];
    } else {
#ifdef VERBOSE
        printf("Error: Missing satellite count in GNGGA message\n");
#endif
        return false;
    }

    return true;
}

bool GNSS::read_data(gnss_data_t *data) {
    int bytes_read = i2c_read_timeout_us(i2c, UBLOX_ADDR, buffer, sizeof(buffer), false, 10000);
    if (bytes_read < 1) {
#ifdef VERBOSE
        printf("Error: Failed to read data from GNSS module\n");
#endif
        data->valid = false;
        return false;
    }

    printf("Received GNSS Data: %.*s\n\n\n\n\n", bytes_read, buffer);

    raw_data = std::string(buffer, buffer + bytes_read);
    data->valid = parse_gngga(data);

    return true;
}

bool GNSS::send_config_msg(int config_msg) {
    // memset(payloadCfg, 0, packetCfgPayloadSize);
    uint8_t msg[17] = {0xB5, 0x62, 0x06, 0x8A, 0x09, 0x00, 0x01, 0x01, 0x00, 0x00, 0x25, 0x00, 0x93, 0x10, 0x00, 0x63, 0xCC};
    i2c_write_blocking(i2c, UBLOX_ADDR, msg, 17, false);
    printf("done writing\n");
    sleep_ms(100);
    return true;
}

void GNSS::calc_ubx_checksum(UBXFrame *frame) {
    uint8_t ck_a = 0;
    uint8_t ck_b = 0;

    ck_a += frame->msg_class;
    ck_b += ck_a;

    ck_a += frame->msg_id;
    ck_b += ck_a;

    ck_a += (frame->len & 0xFF);
    ck_b += ck_a;

    ck_a += (frame->len >> 8);
    ck_b += ck_a;

    for (int i = 0; i < frame->len; i++) {
        ck_a += frame->payload[i];
        ck_b += ck_a;
    }

    frame->ck_a = ck_a;
    frame->ck_b = ck_b;
}

bool GNSS::send_ublox_frame(UBXFrame *frame) {

    calc_ubx_checksum(frame);

    return true;
}