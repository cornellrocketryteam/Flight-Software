/**
 * @file pins.cpp
 * @author csg83
 *
 * @brief Pin initialization functionality
 */

#include "pins.hpp"
#include "constants.hpp"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/spi.h"
#include "hardware/uart.h"
#include "pico/stdlib.h"

void init_pins() {
    // Init LED and turn on immediately
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);
    gpio_put(LED, 1);

    // Init GPIO pins
    gpio_init(ARM_IN);
    gpio_init(SSA_DROGUE);
    gpio_init(SSA_MAIN);
    gpio_init(FRAM_CS);
    gpio_init(SV_SIGNAL);
    gpio_init(BLIMS_ENABLE_PIN);

    gpio_set_dir(ARM_IN, GPIO_IN);
    gpio_set_dir(SSA_DROGUE, GPIO_OUT);
    gpio_set_dir(SSA_MAIN, GPIO_OUT);
    gpio_set_dir(FRAM_CS, GPIO_OUT);
    gpio_set_dir(SV_SIGNAL, GPIO_OUT);
    gpio_set_dir(BLIMS_ENABLE_PIN, GPIO_OUT);

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
    gpio_put(FRAM_CS, 1);

    // Init UART pins
    uart_init(UART_PORT, constants::rfm_baudrate);
    gpio_set_function(RFM_TX, GPIO_FUNC_UART);
    gpio_set_function(RFM_RX, GPIO_FUNC_UART);

    uart_set_format(UART_PORT, 8, 1, UART_PARITY_NONE);

    uart_set_hw_flow(UART_PORT, false, false);
    uart_set_fifo_enabled(UART_PORT, true);

    // Init PWM pins
    gpio_set_function(ARM_OUT, GPIO_FUNC_PWM);
    gpio_set_function(MAV_SIGNAL, GPIO_FUNC_PWM);
    gpio_set_function(BLIMS_PWM_PIN, GPIO_FUNC_PWM);

    const uint buzzer_slice_num = pwm_gpio_to_slice_num(ARM_OUT);
    const uint mav_slice_num = pwm_gpio_to_slice_num(MAV_SIGNAL);

    uint32_t f_sys = clock_get_hz(clk_sys);
    float divider = f_sys / 1000000;
    pwm_set_clkdiv(buzzer_slice_num, divider);

    const uint32_t top = 1000000 / 2048 - 1;
    pwm_set_wrap(buzzer_slice_num, top);

    uint16_t level = (top + 1) * 50 / 100 - 1;
    pwm_set_chan_level(buzzer_slice_num, 1, level);

    const uint8_t mav_divider = 125000000 / 330 / 65535;
    pwm_set_clkdiv_int_frac(mav_slice_num, mav_divider, 12);
    pwm_set_wrap(mav_slice_num, 65535);
    pwm_set_enabled(mav_slice_num, true);

    // Init internal ADC
    adc_init();
    adc_gpio_init(ADC_VSYS);
}