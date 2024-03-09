#include <cstdio>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/rtc.h"
#include "pico/stdlib.h"
#include "pico/util/datetime.h"
#ifndef LAUNCH
#include "tusb.h"
#endif
#ifdef SIM
#include "hardware/uart.h"
#include "pico/multicore.h"
#endif

#include "core/fsw.hpp"
#include "pins.hpp"

Flight flight;

#ifdef SIM
void poll_sim_layer() {
    while (true) {
        uint8_t buf[4];
        uart_read_blocking(UART_ID, buf, 4);
        for (int i = 0; i < 4; i++) {
            printf("byte %d: %d\n", i, buf[i]);
        }

        uint32_t data;
        data = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | (buf[0]);
        printf("data before fifo: %d\n", data);

        multicore_fifo_drain();
        multicore_fifo_push_blocking(data);
    }
}
#endif

int main() {
    stdio_init_all();
#ifdef LAUNCH
    sleep_ms(3000);
    printf("Unplug from computer\n");
    sleep_ms(5000);
    printf("ERROR: Too late. Restart process\n");
    sleep_ms(1000);
#endif

    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    gpio_init(ARMED_IN);
    gpio_set_dir(ARMED_IN, GPIO_IN);

    gpio_init(ARMED_OUT);
    gpio_set_dir(ARMED_OUT, GPIO_OUT);

    gpio_init(SSA_1);
    gpio_set_dir(SSA_1, GPIO_OUT);

    gpio_init(SSA_2);
    gpio_set_dir(SSA_2, GPIO_OUT);

    gpio_init(RFM_CS);
    gpio_set_dir(RFM_CS, GPIO_OUT);
    gpio_put(RFM_CS, 1);

#ifdef SIM
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    multicore_launch_core1(poll_sim_layer);
#endif

#ifndef LAUNCH
    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");
#endif

    while (true) {
        flight.execute();
    }
    return 0;
}
