

#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

#define PWM_PIN 28  // GPIO pin to output PWM signal
// want wrap to be as large as possible, increases the amount of steps so that we have as much control as possible
uint16_t wrap_cycle_count = 65535; // setting to max value for uint16

/*
setup_pwm_50hz configures the pwm signal
takes in gpio_pin
*/
void setup_pwm_50hz(uint gpio_pin) {
    // Set up the PWM configuration
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // What our pico runs - set by the pico hardware itself
    uint32_t clock = 125000000;
    // desired pwm frequency Hz - what we want to achieve
    uint32_t pwm_freq = 50;
    // div is important to get the frequency we want as we lose float info here
    uint32_t divider_int = clock / pwm_freq / wrap_cycle_count;
    // gives us the fractional component of the divider
    uint32_t divider_frac = (clock % (pwm_freq * wrap_cycle_count)) * 16 / (pwm_freq * wrap_cycle_count);
    // Clock divider: slows down pwm to get a certain amount of Hz
    // slice num - depends on what pin we're at, we set pin, pin has set slice num
    // integer divider - going to be 38
    pwm_set_clkdiv_int_frac(slice_num, divider_int, divider_frac);
    pwm_set_wrap(slice_num, wrap_cycle_count);
    pwm_set_enabled(slice_num, true);
}

/*
set_motor_position turns the motor to a position between 0-1
The limits are set by our ODrive configuration -> https://gui.odriverobotics.com/configuration
0 = minimum position
0.5 = neutral
1 = maximum position
*/
void set_motor_position(uint gpio_pin, float position) {
    // Position should be between 0-1
    // Should map between -17 to 17 turns (configured in web UI)
    uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

    // Map position to PWM duty cycle (typically 1ms to 2ms pulse width)
    uint16_t five_percent_duty_cycle = wrap_cycle_count * 0.05;
    // ranges between 5% and 10% duty cycle; 3276 ~= 5% duty, 6552 ~= 10% duty
    uint16_t duty = (uint16_t)(five_percent_duty_cycle + position * five_percent_duty_cycle);
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio_pin), duty);
}

int main() {
    stdio_init_all();

    setup_pwm_50hz(PWM_PIN);

    while (1) {

        set_motor_position(PWM_PIN, 0.0);
        sleep_ms(5000);

        set_motor_position(PWM_PIN, 0.5);
        sleep_ms(5000);

        set_motor_position(PWM_PIN, 1.0);
        sleep_ms(5000);
    }

    return 0;
}

// where is the 800 Hz coming from? AI thought it'd be 50 Hz
// 800 Hz should work with this motor controller?