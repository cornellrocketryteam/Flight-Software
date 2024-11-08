/**
 * @file fsw.hpp
 * @author csg83
 * 
 * @brief The main flight loop
 */

#ifndef FSW_HPP
#define FSW_HPP

// clang-format off
#include "flight_mode.hpp"
#include "pico/stdlib.h"
#include "state.hpp"
#include "constants.hpp"
// clang-format on

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
    uint32_t cycle_start;

     /**
     * The total dynamic time of the current cycle.
     */
    uint32_t cycle_duration;
};

#endif // FSW_HPP