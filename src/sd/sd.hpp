#ifndef SD_HPP_
#define SD_HPP_

#include "f_util.h"
#include "ff.h"
#include "hw_config.h"
#include "rtc.h"

/**
 * Container for SD card-related functionality.
 */
struct SD {
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
     * SD card representation.
     */
    sd_card_t *pSD;

    /**
     * File object representation.
     */
    FIL file;
};

#endif // SD_HPP_