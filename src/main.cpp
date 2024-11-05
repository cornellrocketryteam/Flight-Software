/**
 * @file main.cpp
 * @author csg83
 *
 * @brief The entry point for FSW
 */

#include <cstdio>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"
#ifndef LAUNCH
#include "tusb.h"
#endif

#include "fsw.hpp"
#include "pins.hpp"

Flight flight;

void init_pins() {
    // Init LED and turn on immediately
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 1);

    // Init I2C bus
    i2c_init(I2C_PORT, constants::i2c_baudrate);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Init SPI bus
    spi_init(SPI_PORT, constants::spi_baudrate);
    gpio_set_function(SPI_MISO, GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);

    // Init GPIO pins
    gpio_init(ARM_IN);
    gpio_init(ARM_OUT);
    gpio_init(SSA_DROGUE);
    gpio_init(SSA_MAIN);

    gpio_set_dir(ARM_IN, GPIO_IN);
    gpio_set_dir(ARM_OUT, GPIO_OUT);
    gpio_set_dir(SSA_DROGUE, GPIO_OUT);
    gpio_set_dir(SSA_MAIN, GPIO_OUT);

    // Init UART pins
    uart_init(UART_PORT, constants::rfm_baudrate);
    gpio_set_function(RFM_TX, UART_FUNCSEL_NUM(UART_PORT, RFM_TX));
    gpio_set_function(RFM_RX, UART_FUNCSEL_NUM(UART_PORT, RFM_RX));

    // Init PWM pins
    gpio_set_function(RELAY, GPIO_FUNC_PWM);
    gpio_set_function(MAV, GPIO_FUNC_PWM);
    gpio_set_function(BLIMS_MOTOR, GPIO_FUNC_PWM);

    uint relay_slice_num = pwm_gpio_to_slice_num(RELAY);
    uint mav_slice_num = pwm_gpio_to_slice_num(MAV);
    uint blims_slice_num = pwm_gpio_to_slice_num(BLIMS_MOTOR);
}

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

    while (true) {
        flight.execute();
    }
    return 0;
}
