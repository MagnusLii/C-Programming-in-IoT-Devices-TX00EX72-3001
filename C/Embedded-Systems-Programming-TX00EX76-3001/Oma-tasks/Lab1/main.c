#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define BUTTON_ON_OFF 7
#define BUTTON_INC 8
#define BUTTON_DEC 9

#define N_LED 3
#define STARTING_LED 20
#define LED1 20
#define LED2 21
#define LED3 22

void inc_dutycycle(int *dutycycle){
    if (*dutycycle < 99){
        *dutycycle = *dutycycle + 10;
        if (*dutycycle >= 99)
        {
            *dutycycle = 99;
        }
    }
}

void dec_dutycycle(int *dutycycle){
    if (*dutycycle <= 1){
        *dutycycle = *dutycycle - 10;
        if (*dutycycle <= 1)
        {
            *dutycycle = 1;
        }
    }
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

int main(){
    int dutycycle = 75;

    // setup led(s).
    for (int i = STARTING_LED; i < N_LED; i++){
        gpio_set_function(i, GPIO_FUNC_PWM);
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(22);
        pwm_set_freq_duty(slice_num, chan, 50, dutycycle);
        pwm_set_enabled(slice_num, true);
    }

    // setup button pin 8.
    gpio_init(BUTTON_INC);
    gpio_set_dir(BUTTON_INC, GPIO_IN);
    gpio_pull_up(BUTTON_INC);

    // setup button pin 9.
    gpio_init(BUTTON_DEC);
    gpio_set_dir(BUTTON_DEC, GPIO_IN);
    gpio_pull_up(BUTTON_DEC);

    while (1){
        if (gpio_get(BUTTON_INC) == 0){
            inc_dutycycle(&dutycycle);
            pwm_set_freq_duty(slice_num, chan, 50, dutycycle);
            sleep_ms(100);
        }
        if (gpio_get(BUTTON_DEC) == 0){
            dec_dutycycle(&dutycycle);
            pwm_set_freq_duty(slice_num, chan, 50, dutycycle);
            sleep_ms(100);
        }
    }
    return 0;
}
