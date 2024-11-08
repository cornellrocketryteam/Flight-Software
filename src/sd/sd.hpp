/**
 * @file sd.hpp
 * @author csg83
 * 
 * @brief SD card-related functionality
 */

#ifndef SD_HPP
#define SD_HPP

#include "f_util.h"
#include "ff.h"
#include "hw_config.h"

/**
 * Container for SD card-related functionality.
 */
class SD {
public:
    /**
     * Begins the SD card interface by mounting the SD card.
     * @return True on successful mount, false on mount failure.
     */
    bool begin();

    /**
     * Logs the current state to the SD card.
     * @return True on successful log, false on file open, write, or close failures.
     */
    bool log();

    /**
     * Logs the current flight mode to the boot file.
     * @return True on successful log, false on file open, write, or close failures.
     */
    bool write_mode();

private:
    /**
     * Tracks the number of writes made to the current log file. Resets upon hitting
     * the file writes threshold.
     */
    uint16_t writes_count = 0;

    /**
     * SD card representation.
     */
    sd_card_t *pSD;

    /**
     * File system representation.
     */
    FATFS fs;

    /**
     * Log file object representation.
     */
    FIL log_file;

    /**
     * Boot file object representation.
     */
    FIL boot_file;
};

#endif // SD_HPP