/**
 * @file telem.cpp
 * @author csg83
 *
 * @brief Telemetry-related definitions
 */

#ifndef TELEM_HPP
#define TELEM_HPP

#include "constants.hpp"
#include "pico/stdlib.h"

namespace command {
    constexpr char launch[] = "L";
    constexpr char mav_open[] = "M";
    constexpr char mav_close[] = "m";
    constexpr char sv_open[] = "S";
    constexpr char sv_close[] = "s";

    constexpr char safe[] = "V";

    constexpr char reset_card[] = "D";
    constexpr char reset_fram[] = "F";

    constexpr char reboot[] = "R";

    constexpr char change_target_lat[] = "C1";
    constexpr char change_target_long[] = "C2";
    constexpr char change_ref_pressure[] = "C3";
    constexpr char change_alt_state[] = "C4";
    constexpr char change_card_state[] = "C5";
    constexpr char change_alt_armed[] = "C6";
    constexpr char change_flight_mode[] = "C7";

}; // namespace command

class Telem {
public:
    virtual void transmit() = 0;

protected:
    void pack_data();

    uint16_t packed_metadata = 0;
    uint32_t packed_events = 0;
};

class RFM : Telem {
public:
    void transmit();

private:
    uint8_t packet[constants::rfm_packet_size];
    uint32_t sync_word = constants::sync_word;
};

class Umbilical : Telem {
public:
    void transmit();
    void check_command();

    bool connection_changed();

private:
    void process_command();
    uint8_t packet[constants::umb_packet_size];

    bool receiving = false;
    uint8_t command_index = 0;
    char command_buffer[64];

    uint16_t successful_connections = 0;
    uint16_t failed_connections = 0;
};

#endif // TELEM_HPP