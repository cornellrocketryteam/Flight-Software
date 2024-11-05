#include "hw_config.h"
#include "../../src/pins.hpp"


/* SDIO Interface */
static sd_sdio_if_t sdio_if = {
    .CMD_gpio = SD_CMD,
    .D0_gpio = SD_D0,
    .D1_gpio = SD_D1,
    .D2_gpio = SD_D2,
    .D3_gpio = SD_D3,
    .baud_rate = 125 * 1000 * 1000 / 6  // 20833333 Hz
};

/* Hardware Configuration of the SD Card socket "object" */
static sd_card_t sd_card = {.type = SD_IF_SDIO, .sdio_if_p = &sdio_if};

/**
 * @brief Get the number of SD cards.
 *
 * @return The number of SD cards, which is 1 in this case.
 */
size_t sd_get_num() { return 1; }

/**
 * @brief Get a pointer to an SD card object by its number.
 *
 * @param[in] num The number of the SD card to get.
 *
 * @return A pointer to the SD card object, or @c NULL if the number is invalid.
 */
sd_card_t* sd_get_by_num(size_t num) {
    if (0 == num) {
        // The number 0 is a valid SD card number.
        // Return a pointer to the sd_card object.
        return &sd_card;
    } else {
        // The number is invalid. Return @c NULL.
        return NULL;
    }
}