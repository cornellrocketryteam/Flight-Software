#ifndef RFM_HPP_
#define RFM_HPP_

#include "../pins.hpp"
#include "RadioLib.h"
#include "pico_hal.h"

struct RFM {
    bool begin();
    bool transmit();

    PicoHal *hal = new PicoHal(SPI_PORT);
    SX1276 radio = new Module(hal, RFM_CS, RFM_DIO0, RADIOLIB_NC, RFM_DIO1);
};

#endif // RFM_HPP_