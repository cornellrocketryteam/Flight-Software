#include "hw_config.h"
#include "../pins.hpp"

/* Configuration of RP2040 hardware SPI object */
static spi_t spi = {
    .hw_inst = SPI_PORT,
    .sck_gpio = SPI_SCK,
    .mosi_gpio = SPI_MOSI,
    .miso_gpio = SPI_MISO,
    .baud_rate = 12 * 1000 * 1000 // Actual frequency: 10416666.
};

/* SPI Interface */
static sd_spi_if_t spi_if = {
    .spi = &spi,  // Pointer to the SPI driving this card
    .ss_gpio = SD_CS      // The SPI slave select GPIO for this SD card
};

/* Configuration of the SD Card socket object */
static sd_card_t sd_card = {
    .type = SD_IF_SPI,
    .spi_if_p = &spi_if  // Pointer to the SPI interface driving this card
};

size_t sd_get_num() { return 1; }

sd_card_t *sd_get_by_num(size_t num) {
    if (0 == num) {
        return &sd_card;
    } else {
        return NULL;
    }
}
