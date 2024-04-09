#include <RadioLib.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "../../src/rfm/pico_hal.h"
#include "../../src/pins.hpp"

PicoHal* hal = new PicoHal(SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK, 8000000);

SX1276 radio = new Module(hal, RFM_CS, RFM_DIO0, RADIOLIB_NC, RFM_DIO1);

int transmissionState = RADIOLIB_ERR_NONE;
volatile bool transmittedFlag = false;

void setFlag(void) {
  transmittedFlag = true;
}

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }

    gpio_init(RFM_CS);
    gpio_set_dir(RFM_CS, GPIO_OUT);

    gpio_init(RFM_RST);
    gpio_set_dir(RFM_RST, GPIO_OUT);

    sleep_ms(10);
    gpio_put(RFM_RST, 0);
    sleep_ms(10);
    gpio_put(RFM_RST, 1);

    printf("[SX1276] Initializing ... ");

    int state = radio.begin();
    if (state != RADIOLIB_ERR_NONE) {
        printf("failed, code %d\n", state);
        return 1;
    }
    printf("success!\n");


    radio.setPacketSentAction(setFlag);

    printf("[SX1278] Sending first packet ... ");
    transmissionState = radio.startTransmit("test");

    while (true) {
        if (transmittedFlag) {
            transmittedFlag = false;
            sleep_ms(125);

            if (transmissionState == RADIOLIB_ERR_NONE) {
                printf("transmission finished!");
            } else {
                printf("failed, code %d\n", transmissionState);
            }
            radio.finishTransmit();

            sleep_ms(150);

            printf("[SX1278] Sending another packet ... ");
            transmissionState = radio.startTransmit("test");
        }
    }

}
