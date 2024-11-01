#include <RadioLib.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "../../src/rfm/pico_hal.h"
#include "../../src/pins.hpp"
#include "../../src/constants.hpp"

PicoHal* hal = new PicoHal(SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK, 8000000);

SX1276 radio = new Module(hal, RFM_CS, RFM_DIO0, RADIOLIB_NC, RFM_DIO1);

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }

    gpio_init(RFM_CS);
    gpio_set_dir(RFM_CS, GPIO_OUT);

    gpio_init(RFM_RST);
    gpio_set_dir(RFM_RST, GPIO_OUT);

    gpio_init(TX_EN);
    gpio_set_dir(RFM_CS, TX_EN);
    gpio_put(TX_EN, 1);

    sleep_ms(10);
    gpio_put(RFM_RST, 0);
    sleep_ms(10);
    gpio_put(RFM_RST, 1);

    printf("[SX1276] Initializing ... ");

    int state = radio.begin(constants::frequency, constants::bandwidth, constants::sf, constants::cr, constants::sw, constants::power);
    if (state != RADIOLIB_ERR_NONE) {
        printf("failed, code %d\n", state);
        return 1;
    }
    printf("success!\n");

    state = radio.setFrequency(900);
    if (state != RADIOLIB_ERR_NONE) {
        printf("Set Frequency failed, code %d\n", state);
        return 1;
    }

    while (true) {
        // send a packet
        printf("[SX1276] Transmitting packet ... ");

        state = radio.transmit("test");

        if (state == RADIOLIB_ERR_NONE) {
            // the packet was successfully transmitted
            printf("success!\n");

            // wait for a second before transmitting again
            hal->delay(1000);

        } else {
            printf("failed, code %d\n", state);
        }

    }
    return 0;
}