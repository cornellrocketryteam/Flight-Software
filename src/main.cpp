/**
 * @file main.cpp
 * @author csg83
 *
 * @brief The entry point for FSW
 */

#include "fsw.hpp"
#include "pins.hpp"
#include <cstdio>
#ifndef LAUNCH
#include "tusb.h"
#endif
#include "hardware/watchdog.h"

Flight flight;

int main() {
    stdio_init_all();
#ifdef LAUNCH
    sleep_ms(3000);
    printf("Unplug from computer\n");
    sleep_ms(5000);
    printf("WARNING: Boot file has been modified\n");
    sleep_ms(1000);
#else
    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");
#endif

    init_pins();
    watchdog_enable(constants::watchdog_delay, true);

    while (true) {
        flight.execute();
    }
    return 0;
}
