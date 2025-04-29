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
    /**
     * Transmits a packet over radio
     */
    void transmit();

private:
    /**
     * Buffer for the radio packet
     */
    uint8_t packet[constants::rfm_packet_size];

    /**
     * The syncword used in the radio packet
     */
    uint32_t sync_word = constants::sync_word;
};

class Umbilical : Telem {
public:
    /**
     * Transmits a packet over the umbilical
     */
    void transmit();

    /**
     * Checks the umbilical to see if any commands were received
     */
    void check_command();

    /**
     * Checks the connection of the umbilical and updates
     * the connection locked state if thresholds are reached
     * @return True on a connection lock change, False otherwise
     */
    bool connection_changed();

private:
    /**
     * If a command was received, perform the relevant action
     */
    void process_command();

    /**
     * Buffer for the umbilical packet
     */
    uint8_t packet[constants::umb_packet_size];

    /**
     * Whether we have received the command
     * start flag but not the command end flag
     */
    bool receiving = false;

    /**
     * The buffer to which received commands are written
     */
    char command_buffer[64];

    /**
     * Tracks the current location of received text in command_buffer
     */
    uint8_t command_index = 0;

    /**
     * The number of successful consecutive
     * umbilical connection attempts (1 per cycle)
     */
    uint16_t successful_connections = 0;

    /**
     * The number of failed consecutive
     * umbilical connection attempts (1 per cycle)
     */
    uint16_t failed_connections = 0;
};

#endif // TELEM_HPP