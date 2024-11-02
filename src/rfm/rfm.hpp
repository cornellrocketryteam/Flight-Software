#ifndef RFM_HPP
#define RFM_HPP

#include "RadioLib.h"
#include "pico_hal.h"
#include "pins.hpp"

/**
 * Container for radio module-related functionality.
 */
class RFM {
public:
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

private:
    /**
     * Callback function for when a transmission is finished.
     */
    static void set_flag();

    /**
     * A RadioLib hardware abstraction layer.
     */
    PicoHal *hal = new PicoHal(SPI_PORT, SPI_MISO, SPI_MOSI, SPI_SCK);

    /**
     * A RadioLib radio object representation.
     */
    SX1276 radio = new Module(hal, RFM_CS, RFM_DIO0, RADIOLIB_NC, RFM_DIO1);

    /**
     * Radio return state.
     */
    int state = RADIOLIB_ERR_NONE;
};

#endif // RFM_HPP