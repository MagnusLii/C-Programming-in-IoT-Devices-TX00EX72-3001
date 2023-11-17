#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/util/queue.h"
#include <stdio.h>

#define BUFFER_SIZE 100
#define DEBOUNCE_COUNT 5

#define ROT_A 10
#define ROT_B 11
#define ROT_SW 12

#define BUTTON1_PIN 7
#define BUTTON2_PIN 8
#define BUTTON3_PIN 9
#define SDA_PIN 17
#define SCL_PIN 16

#define N_LED 3
#define STARTING_LED 20
#define LED_BRIGHT_MAX 999
#define LED_BRIGHT_MIN 0
#define LED_BRIGHT_STEP 10

#define EEPROM_ADDR 0x50 // I2C address of the EEPROM
#define BRIGHTNESS_ADDR 2
#define EEPROM_WRITE_DELAY_MS 5
#define LED_STATE_ADDR 32768 // Address in the EEPROM to store the LED state
#define BUFFER_SIZE 1024

static queue_t events;

static void gpio_handler(uint gpio, uint32_t event_mask)
{
    queue_try_add(&events, &gpio);
}

int main()
{
    // Initialize chosen serial port
    stdio_init_all();

    queue_init(&events, sizeof(int), 10);

    int events = 0;
    int value = 0;

        // setup buttons
    for (int i = BUTTON1_PIN; i < BUTTON1_PIN + N_LED; i++){
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
        gpio_pull_up(i);
    }

    gpio_set_irq_enabled_with_callback(BUTTON1_PIN, GPIO_IRQ_EDGE_RISE, true, &gpio_handler);

    while (true)
    {
        while (queue_try_remove(&events, &value))
        {
            printf("Got event: %d, %d\r\n", events, value);
        }
    }
}
