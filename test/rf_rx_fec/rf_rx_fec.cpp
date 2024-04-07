#include <cstring>
#include <string>
#include <iostream>
#include <RadioLib.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "../../lib/RS-FEC.h"
#include "../../src/rfm/pico_hal.h"
#include "../../src/pins.hpp"

#define RX_CS 5
#define RX_RST 0
#define RX_DIO0 13
#define RX_DIO1 14

PicoHal* hal = new PicoHal(SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK, 8000000);

SX1276 radio = new Module(hal, RX_CS, RX_DIO0, RADIOLIB_NC, RX_DIO1);

const int msglen = 86;
const uint8_t ECC_LENGTH = 50;
char encoded[msglen + ECC_LENGTH];
char repaired[msglen];

RS::ReedSolomon<msglen, ECC_LENGTH> rs;

int main() {
    stdio_init_all();

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }

    gpio_init(RX_CS);
    gpio_set_dir(RX_CS, GPIO_OUT);

    gpio_init(RX_RST);
    gpio_set_dir(RX_RST, GPIO_OUT);

    sleep_ms(10);
    gpio_put(RX_RST, 0);
    sleep_ms(10);
    gpio_put(RX_RST, 1);

    printf("[SX1276] Initializing ... ");

    int state = radio.begin();
    if (state != RADIOLIB_ERR_NONE) {
        printf("failed, code %d\n", state);
        return 1;
    }
    printf("success!\n");

    uint8_t str[8];

    while (true) {
        // receive a packet
        printf("[SX1276] Waiting for incoming transmission ... ");

        uint8_t encoded[msglen+ECC_LENGTH];
        int state = radio.receive(encoded, msglen+ECC_LENGTH);
        
        if (state == RADIOLIB_ERR_NONE) {
            // packet was successfully received
            printf("success!");
            // print the data of the packet
            printf("\n[SX1276] Raw Data Length: %d\n[SX1276] Raw Data: ", sizeof(encoded));
            for(uint i = 0; i < sizeof(encoded); i++) {    printf("%c", encoded[i]);    }    printf("\n");

            rs.Decode(encoded, repaired);
            std::string result = repaired;
            printf("Result: \"");
            for (int i=0; i<msglen; i++) printf("%c", (char) repaired[i]);
            printf("\"\n");
            
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