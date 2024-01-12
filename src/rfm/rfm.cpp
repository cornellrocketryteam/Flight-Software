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
#ifdef VERBOSE
        printf("RFM: Init failed, code %d\n", state);
#endif
        return false;
    }
    return true;
}

bool RFM::transmit() {
    int state = radio.transmit("test");

    if (state == RADIOLIB_ERR_NONE) {
#ifdef VERBOSE
        printf("RFM: Transmit success\n");
#endif
        return true;
    }
#ifdef VERBOSE
    printf("RFM: Transmit failed, code %d\n", state);
#endif
    return false;
}