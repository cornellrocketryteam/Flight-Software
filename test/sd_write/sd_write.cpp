#include "pico/stdlib.h"
#include "f_util.h"
#include "ff.h"
#include "hw_config.h"
#include "rtc.h"
#include "tusb.h"
#include "hardware/gpio.h"
#include "../../src/pins.hpp"

int main() {
    stdio_init_all();
    time_init();

    gpio_init(RFM_CS);
    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_put(RFM_CS, 1);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    sd_card_t *pSD = sd_get_by_num(0);

    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr) {
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    printf("Card mounted\n");

    FIL fil;
    const char* const filename = "test.txt";
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);

    if (FR_OK != fr && FR_EXIST != fr) {
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    }
    printf("File opened\n");
        
    if (f_printf(&fil, "Hello, world!\n") < 0) {
        printf("f_printf failed\n");
    }
    printf("Data written\n");

    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    printf("File closed\n");

    f_unmount(pSD->pcName);
    printf("Card unmounted\n");

    return 0;
}