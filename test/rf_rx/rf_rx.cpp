#include <RadioLib.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "../../src/rfm/pico_hal.h"
#include "../../src/pins.hpp"
#include <string>

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

    uint8_t str[8];

    while (true) {
        // send a packet
        printf("[SX1276] Waiting for incoming transmission ... ");

        int state = radio.receive(str, 4);

        if (state == RADIOLIB_ERR_NONE) {
            // packet was successfully received
            printf("success!");

            // print the data of the packet
            printf("[SX1276] Data: %s\n", str);

            // // print the RSSI (Received Signal Strength Indicator)
            // // of the last received packet
            // Serial.print(F("[SX1278] RSSI:\t\t\t"));
            // Serial.print(radio.getRSSI());
            // Serial.println(F(" dBm"));

            // // print the SNR (Signal-to-Noise Ratio)
            // // of the last received packet
            // Serial.print(F("[SX1278] SNR:\t\t\t"));
            // Serial.print(radio.getSNR());
            // Serial.println(F(" dB"));

            // // print frequency error
            // // of the last received packet
            // Serial.print(F("[SX1278] Frequency error:\t"));
            // Serial.print(radio.getFrequencyError());
            // Serial.println(F(" Hz"));

        } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
            // timeout occurred while waiting for a packet
            printf("timeout!\n");
        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            // packet was received, but is malformed
            printf("CRC error!\n");
        } else {
            // some other error occurred
            printf("failed, code %d\n", state);
        }

    }
    return 0;

}