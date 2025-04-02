/**
 * @file main.cpp
 * @author csg83
 *
 * @brief The entry point for FSW
 */

#include "flight_loop.hpp"
#include "pins.hpp"
#include <cstdio>
#ifndef LAUNCH
#include "tusb.h"
#endif
#include "hardware/regs/vreg_and_chip_reset.h"
#include "hardware/watchdog.h"

Flight flight;

int main() {
    stdio_init_all();

#ifdef LAUNCH
    // Determine source of current boot
    int por = 0, wdt = 0;
    uint32_t offset = VREG_AND_CHIP_RESET_BASE + VREG_AND_CHIP_RESET_CHIP_RESET_OFFSET;
    io_rw_32 *reason = (io_rw_32 *)(offset);

    if (*reason & VREG_AND_CHIP_RESET_CHIP_RESET_HAD_POR_BITS) {
        por = 1;
    }
    if (watchdog_caused_reboot()) {
        wdt = 1;
    }

    sleep_ms(3000);
    printf("Power: %d, Watchdog: %d\n", por, wdt);
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
    sleep_ms(15000);

    while (true) {
        flight.execute();
    }
    return 0;
}
