#ifndef FSW_HPP
#define FSW_HPP

#include "../constants.hpp"
#include "flight_mode.hpp"
#include "pico/stdlib.h"
#include "state.hpp"

/**
 * Container for flight level-related functionality.
 */
class Flight {
public:
    /**
     * Executes the current flight mode and checks for a valid transition.
     */
    void execute();

private:
    /**
     * The timestamp of the start of the current cycle.
     */
    uint32_t current_cycle_start;

     /**
     * The total dynamic time of the current cycle.
     */
    uint32_t current_cycle_time;
};

#endif // FSW_HPP