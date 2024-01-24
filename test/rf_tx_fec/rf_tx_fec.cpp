#include <cstring>
#include <string>
#include <iostream>
#include <RadioLib.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "../../lib/RS-FEC.h"
#include "../../src/rfm/pico_hal.h"
#include "../../src/pins.hpp"

const bool ENABLE_FEC = true;

PicoHal* hal = new PicoHal(SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK, 8000000);

SX1276 radio = new Module(hal, RFM_CS, RFM_DIO0, RADIOLIB_NC, RFM_DIO1);

//{"long":0.03,"lat":0.07,"glong":0.0,"glat":0.0,"elev":6534.9}
const int msglen = 61;         //Max message length, and 
const uint8_t ECC_LENGTH = 50;   //Number of guardian bytes, Max corrected bytes = ECC_LENGTH/2
char message_frame[msglen];     //container for shorter messages
char repaired[msglen];
char encoded[msglen + ECC_LENGTH];

RS::ReedSolomon<msglen, ECC_LENGTH> rs;

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


    while (true) {
        printf("Enter message to send:\n");
        printf("> ");
        std::string teststr;
        std::cin >> teststr;
        printf("Your std::string was: \"%s\"\n", teststr);

        if(ENABLE_FEC) {
            int str_len = teststr.length()+1;
            char message[str_len];
            strcpy(message, teststr.c_str());
            printf("Char Array: ");
            printf("%s\n", message);

            memset(message_frame, '-', sizeof(message_frame)); //Clear the array
            for(uint i = 0; i < sizeof(message_frame); i++) {    printf("%c", message_frame[i]);    }    printf("\n");
            for(uint i = 0; i <= str_len; i++) {
                message_frame[i] = message[i];
            }
            for(uint i = 0; i < sizeof(message_frame); i++) {    printf("%c", message_frame[i]);    }    printf("\n");


            rs.Encode(message_frame, encoded);
            printf("Original: "); printf("%s\n", message_frame);
            printf("Encoded:  ");        for(uint i = 0; i < sizeof(encoded); i++) {    printf("%c", encoded[i]);    }    printf("\n");

            printf("Transmitting: ");    for(uint i = 0; i < sizeof(encoded); i++) {    printf("%c", encoded[i]);    }    printf("\n");
            rs.Decode(encoded, repaired);
            std::string result;
            printf("Memcmp result: %d\n", memcmp(message_frame, repaired, msglen));
            if (memcmp(message_frame, repaired, msglen) == 0) {  //Compare the arrays
                result="SUCCESS";
                printf("SUCCESS\n");
            }else {
                result="FAILURE";
                printf("FAILURE\n");
            }
            printf("Result: ");   printf("%s\n", result);
            printf("Repaired: "); printf("%s\n", repaired);


            // send a packet
            printf("Raw Message Size: %d\n", sizeof(encoded));
            printf("[SX1276] Transmitting packet ... ");

            state = radio.transmit(encoded);
        } else {
            printf("[SX1276] Transmitting packet ... ");
            state = radio.transmit("test");
        }

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