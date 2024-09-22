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

    gpio_init(RFM_CS);
    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_put(RFM_CS, 1);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }

    sd_card_t *pSD = sd_get_by_num(0);

    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr) {
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }

    DIR dir;
    FILINFO fno;
    FIL fil;
    FRESULT res;
    char line[256];
    char full_path[FF_MAX_LFN + 1];

    res = f_opendir(&dir, "/");
    if (res != FR_OK) {
        return 1;
    }

    while (true) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) break;
        if (fno.fname[0] == '.') continue;

        res = f_unlink(fno.fname);
        if (res != FR_OK) {
            printf("Failed to delete %s\n", fno.fname);
        }
    }

    f_unmount(pSD->pcName);
    printf("Card unmounted\n");

    return 0;

}