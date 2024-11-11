/**
 * @file fram.hpp
 * @author csg83
 *
 * @brief FRAM-related definitions
 */

#ifndef FRAM_HPP
#define FRAM_HPP

#include "mb85rs.hpp"

enum class Data : uint8_t {
    boot_count = 0,
    flight_mode = 2,
    alt_armed = 3,
    ref_pressure = 4,
    // TODO: PT data during ascent upon SD failure
};

class FRAM {
public:
    FRAM();
    bool begin();

    void write(Data data);

private:
    MB85RS fram;
};

#endif // FRAM_HPP