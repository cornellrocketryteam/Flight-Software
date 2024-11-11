/**
 * @file hw_config.c
 * @author csg83
 *
 * @brief SD hardware configuration options
 */

#include "hw_config.h"
#include "constants.hpp"
#include "pins.hpp"

static sd_sdio_if_t sdio_if = {
    .CMD_gpio = SD_CMD,
    .D0_gpio = SD_D0,
    .baud_rate = constants::sd_baudrate
};

static sd_card_t sd_card = {
    .type = SD_IF_SDIO,
    .sdio_if_p = &sdio_if
};

/**
 * Get the number of SD cards.
 * @return The number of SD cards, which is 1 in this case
 */
size_t sd_get_num() { return 1; }

/**
 * Get a pointer to an SD card object by its number.
 * @param num The number of the SD card to get
 * @return A pointer to the SD card object, or @c NULL if the number is invalid.
 */
sd_card_t *sd_get_by_num(size_t num) {
    if (0 == num) {
        return &sd_card;
    } else {
        return NULL;
    }
}