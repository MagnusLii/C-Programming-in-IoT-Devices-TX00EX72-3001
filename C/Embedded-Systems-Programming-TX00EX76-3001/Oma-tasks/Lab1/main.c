#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdio.h>

#define BUTTON_ON_OFF 8
#define BUTTON_INC 9
#define BUTTON_DEC 7

#define N_LED 3
#define STARTING_LED 20
#define STARTING_DUTYCYCLE 10
#define LED_DUTYCYCLE_STEP 10
#define LED_DUTYCYCLE_MAX 99
#define LED_DUTYCYCLE_MIN 1

void inc_dutycycle(int *dutycycle){
    if (*dutycycle < LED_DUTYCYCLE_MAX){
        *dutycycle = *dutycycle + LED_DUTYCYCLE_STEP;
        if (*dutycycle > LED_DUTYCYCLE_MAX)
        {
            *dutycycle = LED_DUTYCYCLE_MAX;
        }
    }
}

void dec_dutycycle(int *dutycycle){
    if (*dutycycle > LED_DUTYCYCLE_MIN){
        *dutycycle = *dutycycle - LED_DUTYCYCLE_STEP;
        if (*dutycycle < LED_DUTYCYCLE_MIN)
        {
            *dutycycle = 0;
        }
    }
}

uint32_t pwm_set_freq_duty(uint slice_num, uint chan, uint32_t f, int d){
    uint32_t clock = 125000000;
    uint32_t divider16 = clock / f / 4096 + (clock % (f * 4096) != 0);
    if (divider16 / 16 == 0){
        divider16 = 16;
    }
    uint32_t wrap = clock * 16 / divider16 / f - 1;
    pwm_set_clkdiv_int_frac(slice_num, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, wrap * d / 100);
    return wrap;
}

void turn_on_leds(const int dutycycle){
    printf("turn_on_leds: %d\n", dutycycle);
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_freq_duty(slice_num, chan, 50, dutycycle);
    }
}

void turn_off_leds(){
    printf("turn_off_leds\n");
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_freq_duty(slice_num, chan, 50, 0);
    }
}


int main(){
    char OnOff[2][10] = {"OFF", "ON"};
    int dutycycle = STARTING_DUTYCYCLE;

    // setup led(s).
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_freq_duty(slice_num, chan, 50, dutycycle);
        pwm_set_enabled(slice_num, true);
    }
    bool led_state = true;

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

    while (1){
        if (gpio_get(BUTTON_ON_OFF) == 0){
            sleep_ms(250);
            if (led_state == false){
                led_state = true;
                turn_on_leds(dutycycle);
            }
            else if (led_state == true){
                if (dutycycle == 0){
                    turn_on_leds(50);
                    dutycycle = 50;
                }
                else{
                    led_state = false;
                    turn_off_leds();
                }
            }
            while (gpio_get(BUTTON_ON_OFF) == 0);
            printf("Led state: %s\n", OnOff[led_state]);
            sleep_ms(250);
        }
        if (led_state == true && gpio_get(BUTTON_INC) == 0){
            sleep_ms(250);
            inc_dutycycle(&dutycycle);
            turn_on_leds(dutycycle);
            while (gpio_get(BUTTON_INC) == 0);
            sleep_ms(250);
        }
        if (led_state == true && gpio_get(BUTTON_DEC) == 0){
            sleep_ms(250);
            dec_dutycycle(&dutycycle);
            turn_on_leds(dutycycle);
            while (gpio_get(BUTTON_DEC) == 0);
            sleep_ms(250);
        }
    }
}
