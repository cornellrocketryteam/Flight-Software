/**
 * @file fram.hpp
 * @author csg83
 *
 * @brief FRAM-related definitions
 */

#ifndef FRAM_HPP
#define FRAM_HPP

#include "mb85rs.hpp"
#include "types.hpp"

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

private:
    /**
     * The FRAM module
     */
    MB85RS fram;
};

#endif // FRAM_HPP