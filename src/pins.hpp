/**
 * @file pins.hpp
 * @author csg83
 *
 * @brief Pin definitions
 */

#ifndef PINS_HPP
#define PINS_HPP

#define SD_SCK 2
#define SD_CMD 3
#define SD_D0 4
#define SD_D1 5
#define SD_D2 6
#define SD_D3 7

#define UART_PORT uart1
#define RFM_TX 8
#define RFM_RX 9

#define ARM_IN 10
#define ARM_OUT 11

#define I2C_PORT i2c0
#define I2C_SDA 12
#define I2C_SCL 13

#define SSA_DROGUE 14
#define SSA_MAIN 15

#define SPI_PORT spi0
#define SPI_MISO 16
#define SPI_MOSI 19
#define SPI_SCK 18

#define FRAM_CS 17

#define SV_SIGNAL 21
#define MAV_SIGNAL 22

#define LED 25

#define BLIMS_ENABLE_PIN 26
#define BLIMS_PWM_PIN 28

#define ADC_VSYS 29

/**
 * Initializes all GPIO, I2C, SPI, UART, and PWM pins
 */
void init_pins();

#endif // PINS_HPP