#ifndef FSW_HPP_
#define FSW_HPP_

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
};

#endif // FSW_HPP_