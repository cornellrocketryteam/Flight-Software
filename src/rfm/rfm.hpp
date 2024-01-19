#ifndef RFM_HPP_
#define RFM_HPP_

#include "../pins.hpp"
#include "RadioLib.h"
#include "pico_hal.h"

/**
 * Container for radio module-related functionality.
 */
struct RFM {
    /**
     * Begins the radio interface by initializing the RadioLib radio object.
     * @return True on successful initialization, false on failure.
     */
    bool begin();

    /**
     * Transmits the current state formatted as a standardized packet.
     * @return True on transmit success, false on transmit failure.
     */
    bool transmit();

    /**
     * A RadioLib hardware abstraction layer.
     */
    PicoHal *hal = new PicoHal(SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK);

    /**
     * A RadioLib radio object representation.
     */
    SX1276 radio = new Module(hal, RFM_CS, RFM_DIO0, RADIOLIB_NC, RFM_DIO1);
};

#endif // RFM_HPP_