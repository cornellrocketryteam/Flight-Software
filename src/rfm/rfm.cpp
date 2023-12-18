#include "rfm.hpp"

bool RFM::begin() {
    gpio_init(RFM_CS);
    gpio_init(RFM_RST);

    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_set_dir(RFM_RST, GPIO_OUT);

    sleep_ms(10);
    gpio_put(RFM_RST, 0);
    sleep_ms(10);
    gpio_put(RFM_RST, 1);

    int state = radio.begin();
    if (state != RADIOLIB_ERR_NONE) {
        printf("failed, code %d\n", state);
        return false;
    }
    return true;
}

bool RFM::transmit() {
    return true;
}