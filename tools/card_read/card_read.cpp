/**
 * @file read_card.cpp
 * @author csg83
 * 
 * @brief Reads out the contents of the SD card into a single file
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

    FATFS fs;
    DIR dir;
    FIL fil;
    FILINFO fno;
    char line[256];

    FRESULT fr = f_mount(&fs, "", 1);
    if (fr != FR_OK) {
        printf("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
        return 1;
    }

    fr = f_opendir(&dir, "/");
    if (fr != FR_OK) {
        printf("f_opendir error: %s (%d)\n", FRESULT_str(fr), fr);
        return 1;
    }

    while (true) {
        fr = f_readdir(&dir, &fno);
        if (fr != FR_OK || fno.fname[0] == 0) break;
        if (fno.fname[0] == '.') continue;

        if (strcmp(fno.fname, "boot.txt") != 0 && (fno.fattrib & AM_DIR) == 0) {
            printf("Opening %s...\n", fno.fname);

            fr = f_open(&fil, fno.fname, FA_READ);
            if (fr == FR_OK) {
                while (f_gets(line, sizeof(line), &fil)) {
                    printf("%s", line);
                }
                f_close(&fil);
            } else {
                printf("Failed to open %s\n", fno.fname);
            }
        }
    }

    f_unmount("");
    printf("Card unmounted\n");

    return 0;
}