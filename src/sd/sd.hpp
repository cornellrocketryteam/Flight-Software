#ifndef SD_HPP_
#define SD_HPP_

#include "f_util.h"
#include "ff.h"
#include "hw_config.h"
#include "rtc.h"

struct SD {
    bool begin();
    bool log();

    sd_card_t *pSD;
    FIL file;
};

#endif // SD_HPP_