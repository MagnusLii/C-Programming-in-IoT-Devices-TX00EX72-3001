#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define BUTTON_ON_OFF 7
#define BUTTON_INC 8
#define BUTTON_DEC 9

#define LED1 20
#define LED2 21
#define LED3 22

bool led_state = false;
uint brightness = 128; // Start at 50% brightness

void toggle_leds() {
    led_state = !led_state;
    if (led_state) {
        pwm_set_gpio_level(LED1, brightness);
        pwm_set_gpio_level(LED2, brightness);
        pwm_set_gpio_level(LED3, brightness);
    } else {
        pwm_set_gpio_level(LED1, 0);
        pwm_set_gpio_level(LED2, 0);
        pwm_set_gpio_level(LED3, 0);
    }
}

void increase_brightness() {
    if (brightness < 255) brightness++;
}

void decrease_brightness() {
    if (brightness > 0) brightness--;
}

int main() {
    stdio_init_all();

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
