#ifndef FSW_HPP_
#define FSW_HPP_

#include "flight_mode.hpp"
#include "state.hpp"
#include "pico/stdlib.h"

/**
 * Container for flight-level related functionality.
 * 
*/
class Flight {
public:
    void execute();
};

#endif // FSW_HPP_