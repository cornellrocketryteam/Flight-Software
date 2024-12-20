#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "constants.hpp"
#include "pins.hpp"
#include <cstring>

const char *command_enter_seq = "+++";
const char *save_values_seq = "AT&W\r";
const char *reboot_seq = "ATZ\r";

void set_register(uint8_t register_num, uint8_t value) {
    uart_write_blocking(UART_PORT, (const uint8_t *)command_enter_seq, strlen(command_enter_seq));
    sleep_ms(1000);

    char command[20];
    sprintf(command, "ATS%d=%d\r", register_num, value);
    uart_write_blocking(UART_PORT, (const uint8_t *)command, strlen(command));

    uart_write_blocking(UART_PORT, (const uint8_t *)save_values_seq, strlen(save_values_seq));
    sleep_ms(100);

    uart_write_blocking(UART_PORT, (const uint8_t *)reboot_seq, strlen(reboot_seq));
}

int main() {
    stdio_init_all();

    uart_init(UART_PORT, 115200);

    gpio_set_function(RFM_TX, GPIO_FUNC_UART);
    gpio_set_function(RFM_RX, GPIO_FUNC_UART);

    uart_set_format(UART_PORT, 8, 1, UART_PARITY_NONE);

    uart_set_hw_flow(UART_PORT, false, false);
    uart_set_fifo_enabled(UART_PORT, true);

    int i = 0;
    char buffer[107];

    // set_register(4, 0);



    while (true) {
        sprintf(buffer, "ZachZachZachZachZachZachZachZachZachZachZachZachZachZachZachZachZachZachZachZachZachZachZach%d!!\n", i);
        uart_write_blocking(UART_PORT, (const uint8_t *)buffer, strlen(buffer));
        i++;
        sleep_ms(20);
    }

    return 0;
}
