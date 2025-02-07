/**
 * @file sd.hpp
 * @author csg83
 *
 * @brief SD card-related definitions
 */

#ifndef SD_HPP
#define SD_HPP

#include "f_util.h"
#include "ff.h"
#include "hw_config.h"
#include <sstream>

/**
 * Container for SD card-related functionality.
 */
class SD {
public:
    /**
     * Begins the SD card interface by mounting the SD card.
     */
    void begin();

    /**
     * Logs the current state to the SD card.
     */
    void log();

    /**
     * Clears all data from the SD card.
     * @return True on successful clear, false on file open or close failures
     */
    bool clear_card();

private:
    /**
     * The string to which all logged data will be appended
     */
    std::string data;

    /**
     * Tracks the number of writes made to the current log file. Resets upon hitting
     * the file writes threshold.
     */
    uint16_t writes_count = 0;

    /**
     * File system representation
     */
    FATFS fs;

    /**
     * Return value for SD operations
     */
    FRESULT fr;

    /**
     * Log file object representation
     */
    FIL log_file;
};

#endif // SD_HPP