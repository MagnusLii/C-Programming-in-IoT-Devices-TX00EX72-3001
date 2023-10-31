#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

#define ROT_A 10
#define ROT_B 11
#define ROT_SW 12

#define N_LED 3
#define STARTING_LED 20
#define LED_BRIGHT_MAX 999
#define LED_BRIGHT_MIN 0

volatile bool led_state = true;
volatile uint brightness = 500;
volatile bool status_changed = false;

void turn_on_leds(const int dutycycle){
    printf("turn_on_leds: %d\n", dutycycle);
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_chan_level(slice_num, chan, dutycycle);
    }
}

void turn_off_leds(){
    printf("turn_off_leds\n");
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_chan_level(slice_num, chan, 0);
    }
}

void gpio_callback(uint gpio, uint32_t events){
    if (gpio == ROT_A){
        if (gpio_get(ROT_B)) {
            if (brightness < LED_BRIGHT_MAX) brightness++;
        } else {
            if (brightness > LED_BRIGHT_MIN) brightness--;
        }
    } else if (gpio == ROT_SW){
        led_state = !led_state;
    }
    status_changed = true;
}

int main(){
    char OnOff[2][10] = {"OFF", "ON"};

    // setup led(s).
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_enabled(slice_num, false);
        pwm_config config = pwm_get_default_config();
        pwm_config_set_clkdiv_int(&config, 125);
        pwm_config_set_wrap(&config, 1000); // 1kHz
        pwm_init(slice_num, &config, false);
        pwm_set_chan_level(slice_num, chan, 500); // 50% duty cycle
        gpio_set_function(i, GPIO_FUNC_PWM);
        pwm_set_enabled(slice_num, true);
    }

    // setup button pin for on/off.
    gpio_init(ROT_SW);
    gpio_set_dir(ROT_SW, GPIO_IN);
    gpio_pull_up(ROT_SW);

    // setup button pin for increase.
    gpio_init(ROT_A);
    gpio_set_dir(ROT_A, GPIO_IN);
    gpio_pull_up(ROT_A);

    // setup button pin for decrease.
    gpio_init(ROT_B);
    gpio_set_dir(ROT_B, GPIO_IN);
    gpio_pull_up(ROT_B);

    gpio_set_irq_enabled_with_callback(ROT_A, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(ROT_SW, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    stdio_init_all();

    while (1) {
        if (led_state && status_changed == true) {
            turn_on_leds(brightness);
            printf("Status: %s, brightness: %d\n", OnOff[led_state], brightness);
            status_changed = false;            
        } else if (!led_state && status_changed == true){
            sleep_ms(100);
            turn_off_leds();
            printf("Status: %s\n", OnOff[led_state]);
            status_changed = false;
            sleep_ms(100);
        }
    }
    return 0;
}