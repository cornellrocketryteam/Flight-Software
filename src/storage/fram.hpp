/**
 * @file fram.hpp
 * @author csg83
 *
 * @brief FRAM-related definitions
 */

#ifndef FRAM_HPP
#define FRAM_HPP

#include "mb85rs.hpp"

class FRAM {
public:
    FRAM();
    bool begin();

    void write_mode();

private:
    MB85RS fram;
};

#endif // FRAM_HPP