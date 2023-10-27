#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

#define BUTTON_ON_OFF 8
#define BUTTON_INC 9
#define BUTTON_DEC 7

#define N_LED 3
#define STARTING_LED 20

void inc_dutycycle(int *dutycycle){
    printf("inc_dutycycle\n");
    if (*dutycycle < 99){
        *dutycycle = *dutycycle + 10;
        if (*dutycycle >= 99)
        {
            *dutycycle = 99;
        }
    }
    printf("dutycycle: %d\n", *dutycycle);
}

void dec_dutycycle(int *dutycycle){
    printf("dec_dutycycle\n");
    if (*dutycycle > 1){
        *dutycycle = *dutycycle - 10;
        if (*dutycycle <= 1)
        {
            *dutycycle = 0;
        }
    }
    printf("dutycycle: %d\n", *dutycycle);
}

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d){
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
    if (divider16 / 16 == 0)
        divider16 = 16;

    uint32_t wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}

void toggle_leds(bool led_state, const int dutycycle){
    printf("toggle_leds\n");
    led_state = !led_state;
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        if(led_state) {
            pwm_set_freq_duty(slice_num, chan, 50, dutycycle);
            pwm_set_enabled(slice_num, true);
        } else {
            pwm_set_enabled(slice_num, false);
        }
    }
}

int main(){
    int dutycycle = 75;
    bool led_state = false;

    // setup led(s).
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        printf("setup led %d\n", i);
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_freq_duty(slice_num, chan, 50, dutycycle);
    }

    // setup button pin for on/off.
    gpio_init(BUTTON_ON_OFF);
    gpio_set_dir(BUTTON_ON_OFF, GPIO_IN);
    gpio_pull_up(BUTTON_ON_OFF);

    // setup button pin for increase.
    gpio_init(BUTTON_INC);
    gpio_set_dir(BUTTON_INC, GPIO_IN);
    gpio_pull_up(BUTTON_INC);

    // setup button pin for decrease.
    gpio_init(BUTTON_DEC);
    gpio_set_dir(BUTTON_DEC, GPIO_IN);
    gpio_pull_up(BUTTON_DEC);

    stdio_init_all();

    printf("start loop\n");
    while (1){
        if (gpio_get(BUTTON_ON_OFF) == 0){
            toggle_leds(led_state, dutycycle);
        }
        if (gpio_get(BUTTON_INC) == 0){
            inc_dutycycle(&dutycycle);
            toggle_leds(led_state, dutycycle);
        }
        if (gpio_get(BUTTON_DEC) == 0){
            dec_dutycycle(&dutycycle);
            toggle_leds(led_state, dutycycle);
        }
    }
}
