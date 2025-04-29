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
    old_mode = 2,
    watchdog_boot_count = 3,
    alt_armed = 4,
    ref_pressure = 5,
    mav_state = 9,
    sv_state = 10,
    pt_index = 11,
    pt = 13
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
    void begin();

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

    /**
     * Resets all data in FRAM
     */
    void reset_data();

private:
    /**
     * The FRAM module
     */
    MB85RS fram;

    /**
     * Data that will be written to FRAM upon a reset
     */
    uint8_t clear[11] = {0};
};

#endif // FRAM_HPP