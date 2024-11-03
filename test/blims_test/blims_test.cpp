

#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define PWM_PIN 26  // GPIO pin to output PWM signal

void setup_pwm_50hz(uint gpio_pin) {
    // Set up the PWM configuration
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // Configure PWM for 50 Hz
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / 50 / 65535;
    // Clock divider: slows down pwm to get a certain amount of Hz
    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice_num, 65535);
    pwm_set_enabled(slice_num, true);
}

void set_servo_position(uint gpio_pin, float position) {
    // Position should be between 0.0 (0 degrees) and 1.0 (180 degrees)
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // Map position to PWM duty cycle (typically 1ms to 2ms pulse width)
    uint16_t duty = (uint16_t)(position * 65535);  // 1638 ~= 2.5% duty, 4915 ~= 7.5% duty
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio_pin), duty);
}

int main() {
    stdio_init_all();

    setup_pwm_50hz(PWM_PIN);

    while (1) {
        // Move servo to 0 degrees
        set_servo_position(PWM_PIN, 0.0);
        sleep_ms(5000);

        // Move servo to 90 degrees
        set_servo_position(PWM_PIN, 0.5);
        sleep_ms(5000);

        // Move servo to 180 degrees
        set_servo_position(PWM_PIN, 1.0);
        sleep_ms(5000);
    }

    return 0;
}

// where is the 800 Hz coming from? AI thought it'd be 50 Hz
// 800 Hz should work with this motor controller?