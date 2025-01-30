/**
 * @file buzzer.cpp
 * @author csg83
 * 
 * @brief Unit test of the CFC's buzzer
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pins.hpp"

void buzz(const uint32_t length) {
	pwm_set_enabled(buzzer_slice_num, true);
	sleep_ms(length);
	pwm_set_enabled(buzzer_slice_num, false);
	sleep_ms(30);
}

int main() {
    stdio_init_all();

	init_pins();

	while (true) {
		buzz(250);
		buzz(100);
		buzz(50);
		buzz(50);
		buzz(20);
		buzz(70);
		buzz(10);
		buzz(100);
		buzz(250);
	}
	
	return 0;
}