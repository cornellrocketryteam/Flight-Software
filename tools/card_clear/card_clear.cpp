/**
 * @file clear_card.cpp
 * @author csg83
 * 
 * @brief Clears the contents of the SD card
 */

#include "pico/stdlib.h"
#include "f_util.h"
#include "ff.h"
#include "hw_config.h"
#include "tusb.h"
#include "hardware/gpio.h"
#include "../../src/pins.hpp"

int main() {
    stdio_init_all();
    
    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    FATFS fs;
    DIR dir;
    FILINFO fno;

    FRESULT fr = f_mount(&fs, "", 1);

    fr = f_opendir(&dir, "/");
    if (fr != FR_OK && fr != FR_EXIST) {
        printf("SD Open Error: %s (%d)\n", FRESULT_str(fr), fr);
        return false;
    }

    while (true) {
        fr = f_readdir(&dir, &fno);
        if (fr != FR_OK || fno.fname[0] == 0) break;
        if (fno.fname[0] == '.') continue;

        printf("Deleting %s\n", fno.fname);
        fr = f_unlink(fno.fname);
        if (fr != FR_OK) {
            printf("SD Delete Error: Failed to delete %s\n", fno.fname);
            return false;
        }
    }

    f_unmount("");
    printf("Card unmounted\n");

    return 0;
}