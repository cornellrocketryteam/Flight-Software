/**
 * @file actuate_ssa.cpp
 * @author csg83
 * 
 * @brief Unit test of SSA actuation for both main and drogue deployment
 */

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "tusb.h"
#include "pins.hpp"
#include "constants.hpp"

enum class Chute : uint8_t {
    drogue = SSA_DROGUE,
    main = SSA_MAIN
};

int64_t callback(alarm_id_t id, void *user_data) {
    Chute chute = static_cast<Chute>(reinterpret_cast<uintptr_t>(user_data));
    gpio_put(static_cast<uint8_t>(chute), 0);
#ifdef VERBOSE
    printf("Ematch set LOW\n");
#endif
    return 0;
}

void trigger(Chute chute) {
    gpio_put(static_cast<uint8_t>(chute), 1);
#ifdef VERBOSE
    printf("Ematch set HIGH\n");
#endif
    add_alarm_in_ms(constants::ematch_threshold, callback, reinterpret_cast<void *>(static_cast<uintptr_t>(chute)), true);
}

int main() {
    stdio_init_all();

    gpio_init(SSA_DROGUE);
    gpio_init(SSA_MAIN);

    gpio_set_dir(SSA_DROGUE, GPIO_OUT);
    gpio_set_dir(SSA_MAIN, GPIO_OUT);

#ifdef VERBOSE
    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");
#endif

    sleep_ms(10000);
    trigger(Chute::drogue);
    
    sleep_ms(10000);
    trigger(Chute::main);

    sleep_ms(1000);

    return 0;
}