#include "sd.hpp"
#include "../constants.hpp"
#include <cstdio>
#include "../core/state.hpp"

bool SD::begin() {
    time_init();

    pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);

    if (fr != FR_OK) {
        return false;
    }

    return true;
}

bool SD::log() {
    // Temporary dummy data
    // char* log = "GYROX,GYROY,GYROZ,MAGX,MAGY,MAGZ,ACCELX,ACCELY,ACCELZ,TEMP,HUM,ALT,GPS1,GPS2,GPS3,GPS4,GPS5,GPS6\n"; 
    // sprintf(log, "%d", state::flight::cycle_count);

    FRESULT fr = f_open(&file, constants::sd::filename, FA_OPEN_APPEND | FA_WRITE);

    if (fr != FR_OK && fr != FR_EXIST) {
        return false;
    }

    if (f_printf(&file, "%d\n", state::flight::cycle_count) < 0) {
        return false;
    }

    fr = f_close(&file);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    return true;
}