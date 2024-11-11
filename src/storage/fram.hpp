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
    /**
     * Initializes the underlying FRAM object
     */
    FRAM();

    /**
     * Begins the FRAM module and loads metadata to state
     */
    bool begin();

    /**
     * Loads data from FRAM into state
     * @param data The data to load
     */
    void load(Data data);

    /**
     * Stores data from state into FRAM
     * @param data The data to store
     */
    void store(Data data);

private:
    /**
     * The FRAM module
     */
    MB85RS fram;
};

#endif // FRAM_HPP