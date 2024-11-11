#include "hw_config.h"
#include "../../src/pins.hpp"

#ifdef SDIO
/* SDIO Interface */
static sd_sdio_if_t sdio_if = {
    .CMD_gpio = 3,
    .D0_gpio = 4,
    // .baud_rate = 125 * 1000 * 1000 / 6  // 20833333 Hz
};

static sd_card_t sd_card = {
    .type = SD_IF_SDIO,
    .sdio_if_p = &sdio_if
};
#else
static spi_t spi = {
    .hw_inst = SPI_PORT,
    .sck_gpio = SPI_SCK,
    .mosi_gpio = SPI_MOSI,
    .miso_gpio = SPI_MISO,
    .baud_rate = 12 * 1000 * 1000 // Actual frequency: 10416666.
};

/* SPI Interface */
static sd_spi_if_t spi_if = {
    .spi = &spi,
    .ss_gpio = 17
};

static sd_card_t sd_card = {
    .type = SD_IF_SPI,
    .spi_if_p = &spi_if
};
#endif

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