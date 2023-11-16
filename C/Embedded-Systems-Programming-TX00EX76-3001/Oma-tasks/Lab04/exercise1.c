#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include <stdio.h>

#define ROT_A 10
#define ROT_B 11
#define ROT_SW 12

#define N_LED 3
#define STARTING_LED 20
#define LED_BRIGHT_MAX 999
#define LED_BRIGHT_MIN 0
#define LED_BRIGHT_STEP 10

#define EEPROM_ADDR 0x50  // I2C address of the EEPROM
#define LED_STATE_ADDR 0xFFFF  // Address in the EEPROM to store the LED state
#define SDA_PIN 21
#define SCL_PIN 22

volatile bool led_state = true;
volatile uint brightness = 500;
volatile bool status_changed = false;
volatile bool led_status_changed = false;

// Structure to hold the LED state and its inverted value
typedef struct ledstate {
    bool state;  // The actual state of the LEDs
    bool not_state;  // The inverted state of the LEDs
} ledstate;

void set_led_state(ledstate *ls, uint8_t value);
bool led_state_is_valid(ledstate *ls);
void write_led_state_to_eeprom(ledstate *ls);
void read_led_state_from_eeprom(ledstate *ls);
void change_bright();
void toggle_leds();
void gpio_callback(uint gpio, uint32_t events);


int main(){
    stdio_init_all();

    // init the I2C bus
    i2c_init(i2c_default, 100 * 1000); //100khz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    char OnOff[2][10] = {"OFF", "ON"};

    // setup led(s).
    for (int led_pin = STARTING_LED; led_pin < STARTING_LED + N_LED; led_pin++){
        uint slice_num = pwm_gpio_to_slice_num(led_pin);
        pwm_set_enabled(slice_num, false);
        pwm_config config = pwm_get_default_config();
        pwm_config_set_clkdiv_int(&config, 125);
        pwm_config_set_wrap(&config, 1000); // 1kHz
        pwm_init(slice_num, &config, false);
        gpio_set_function(led_pin, GPIO_FUNC_PWM);
        pwm_set_enabled(slice_num, true);
    }
    change_bright();

    // setup button pin for on/off.
    gpio_init(ROT_SW);
    gpio_set_dir(ROT_SW, GPIO_IN);
    gpio_pull_up(ROT_SW);

    // setup button pin for increase.
    gpio_init(ROT_A);
    gpio_set_dir(ROT_A, GPIO_IN);

    // setup button pin for decrease.
    gpio_init(ROT_B);
    gpio_set_dir(ROT_B, GPIO_IN);

    gpio_set_irq_enabled_with_callback(ROT_A, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(ROT_SW, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Read the LED state from the EEPROM
    ledstate ls;
    read_led_state_from_eeprom(&ls);

    // If the LED state is not valid, set all LEDs on and write to EEPROM
    if (!led_state_is_valid(&ls)) {
        led_state = true;
        set_led_state(&ls, true);  // LEDs on
        write_led_state_to_eeprom(&ls);
    }

    read_led_state_from_eeprom(&ls);
    led_state = ls.state;
    led_status_changed = true;

    while (1) {
        if (status_changed == true){
            if (led_state != false){
                change_bright();
                printf("Brightness: %d\n", brightness);
            }
            status_changed = false;
        }
        if (led_status_changed == true){
            toggle_leds();
            printf("LEDs: %s\n", OnOff[led_state]);
            led_status_changed = false;
        }
    }
    return 0;
}


// Function to set the LED state and its inverted value in the ledstate structure
void set_led_state(ledstate *ls, bool value) {
    ls->state = value;
    ls->not_state = !value;
}

// Function to check if the LED state and its inverted value match
bool led_state_is_valid(ledstate *ls) {
    return ls->state == !ls->not_state;
}

// Function to write the LED state to the EEPROM
void write_led_state_to_eeprom(ledstate *ls) {
    uint8_t data[2] = {ls->state, ls->not_state};
    uint8_t reg_addr[2] = {LED_STATE_ADDR >> 8, LED_STATE_ADDR & 0xFF};  // High and low bytes of the EEPROM address
    uint8_t combined[4] = {reg_addr[0], reg_addr[1], data[0], data[1]};  // Combine the register address and data
    i2c_write_blocking(i2c_default, EEPROM_ADDR, combined, 4, false);
}

// Function to read the LED state from the EEPROM
void read_led_state_from_eeprom(ledstate *ls) {
    uint8_t reg_addr[2] = {LED_STATE_ADDR >> 8, LED_STATE_ADDR & 0xFF};  // High and low bytes of the EEPROM address
    i2c_write_blocking(i2c_default, EEPROM_ADDR, reg_addr, 2, true);  // Write the register address with nostop=true
    uint8_t data[2];
    i2c_read_blocking(i2c_default, EEPROM_ADDR, data, 2, false);
    ls->state = data[0];
    ls->not_state = data[1];
}

void change_bright(){
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++){
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_chan_level(slice_num, chan, brightness);
    }
}

void toggle_leds(){
    if (brightness == 0 && led_state == true){
        brightness = 500;
        change_bright();
    } else if (led_state == false){
        led_state = true;
        change_bright();
    } else if (led_state == true){
        led_state = false;
        for (int led_pin = STARTING_LED; led_pin < STARTING_LED + N_LED; led_pin++){
            uint slice_num = pwm_gpio_to_slice_num(led_pin);
            uint chan = pwm_gpio_to_channel(led_pin);
            pwm_set_chan_level(slice_num, chan, 0);
        }
    }
}

void gpio_callback(uint gpio, uint32_t events){
    int debounce_counter = 0;
    
    if (gpio == ROT_A){
        if (gpio_get(ROT_B)) {
            if (brightness > LED_BRIGHT_MIN){
                brightness -= LED_BRIGHT_STEP;
            }
        } else {
            if (brightness < LED_BRIGHT_MAX){
                brightness += LED_BRIGHT_STEP;
            }
        }
        status_changed = true;
    } 
    
    else if (gpio == ROT_SW && led_status_changed == false){
        led_status_changed = true;

        //clear release debounce.
        while (debounce_counter < 100000)
        {
            if (gpio_get(ROT_SW) == 1){
                debounce_counter++;
            } else {
                debounce_counter = 0;
            }
        }
    }
}