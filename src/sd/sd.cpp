#include "sd.hpp"
#include "../constants.hpp"
#include "../core/state.hpp"
#include <cstdio>

bool SD::begin() {
    time_init();

    pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);

    if (fr != FR_OK) {
#ifdef VERBOSE
        printf("SD: %s (%d)\n", FRESULT_str(fr), fr);
#endif
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
#ifdef VERBOSE
        printf("SD: %s (%d)\n", FRESULT_str(fr), fr);
#endif
        return false;
    }

    if (f_printf(&file, "%d\n", state::flight::cycle_count) < 0) {
#ifdef VERBOSE
        printf("SD: %s (%d)\n", FRESULT_str(fr), fr);
#endif
        return false;
    }

    fr = f_close(&file);
    if (FR_OK != fr) {
#ifdef VERBOSE
        printf("SD: %s (%d)\n", FRESULT_str(fr), fr);
#endif

        return false;
    }
#ifdef VERBOSE
    printf("SD: Log success\n");
#endif

    return true;
}