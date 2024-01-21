#include "rfm.hpp"

volatile bool awaiting = true;

void RFM::set_flag() {
    awaiting = true;
}

bool RFM::begin() {
    gpio_init(RFM_CS);
    gpio_init(RFM_RST);

    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_set_dir(RFM_RST, GPIO_OUT);

    sleep_ms(10);
    gpio_put(RFM_RST, 0);
    sleep_ms(10);
    gpio_put(RFM_RST, 1);

    state = radio.begin();
    if (state != RADIOLIB_ERR_NONE) {
#ifdef VERBOSE
        printf("RFM Error: Init failed, code %d\n", state);
#endif
        return false;
    }

    radio.setPacketSentAction(set_flag);

    return true;
}

bool RFM::transmit() {
    if (awaiting) {
        awaiting = false;

        if (state == RADIOLIB_ERR_NONE) {
#ifdef VERBOSE
            printf("RFM: Transmit success\n");
#endif
        } else {
#ifdef VERBOSE
            printf("RFM Error: Transmit failed, code %d\n", state);
            return false;
        }
#endif
        radio.finishTransmit();

        state = radio.startTransmit("test");
        return true;
    }
    return false;
}