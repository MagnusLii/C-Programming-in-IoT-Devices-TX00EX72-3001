#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define BUTTON_ON_OFF 7
#define BUTTON_INC 8
#define BUTTON_DEC 9

#define LED1 20
#define LED2 21
#define LED3 22

int main()
{
    
    gpio_set_function(LED1, GPIO_FUNC_PWM);
    gpio_set_function(LED2, GPIO_FUNC_PWM);
    gpio_set_function(LED3, GPIO_FUNC_PWM);

    uint slice_num1 = pwm_gpio_to_slice_num(LED1);
    uint slice_num2 = pwm_gpio_to_slice_num(LED2);
    uint slice_num3 = pwm_gpio_to_slice_num(LED3);

    pwm_set_enabled(slice_num1, true);
    pwm_set_enabled(slice_num2, true);
    pwm_set_enabled(slice_num3, true);

    pwm_set_wrap(slice_num1, 255);
    pwm_set_wrap(slice_num2, 255);
    pwm_set_wrap(slice_num3, 255);

    pwm_set_chan_level(slice_num1, PWM_CHAN_A, 0);
    pwm_set_chan_level(slice_num2, PWM_CHAN_A, 0);
    pwm_set_chan_level(slice_num3, PWM_CHAN_A, 0);

    // Initialize buttons
    gpio_init(BUTTON_ON_OFF);
    gpio_init(BUTTON_INC);
    gpio_init(BUTTON_DEC);

    gpio_set_dir(BUTTON_ON_OFF, GPIO_IN);
    gpio_set_dir(BUTTON_INC, GPIO_IN);
    gpio_set_dir(BUTTON_DEC, GPIO_IN);

    // Initialize LEDs
    gpio_init(LED1);
    gpio_init(LED2);
    gpio_init(LED3);

    gpio_set_dir(LED1, GPIO_OUT);
    gpio_set_dir(LED2, GPIO_OUT);
    gpio_set_dir(LED3, GPIO_OUT);

    while (true) {
        if (gpio_get(BUTTON_ON_OFF)) {
            toggle_leds();
            sleep_ms(200); // Debounce delay
        }

        if (led_state) {
            if (gpio_get(BUTTON_INC)) {
                increase_brightness();
                sleep_ms(10); // Smooth dimming delay
            }

            if (gpio_get(BUTTON_DEC)) {
                decrease_brightness();
                sleep_ms(10); // Smooth dimming delay
            }
        }
    }

    return 0;
}

