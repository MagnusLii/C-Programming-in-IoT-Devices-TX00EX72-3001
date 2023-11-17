#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/util/queue.h"
#include <stdio.h>

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
#define BUFFER_SIZE 512

static void gpio_callback(uint gpio, uint32_t event_mask);
void toggleLED(uint gpioPin, struct ledStatus *ledStatusStruct);
void incBrightness(struct ledStatus *ledStatusStruct);
void decBrightness(struct ledStatus *ledStatusStruct);

typedef struct ledStatus
{
    bool ledState[3];
    uint16_t brightness;
} ledStatus;

typedef struct queueTrain
{
    uint16_t gpioPin;
    uint16_t pinState;
} queueTrain;

static queue_t irqEvents;

int main()
{
    stdio_init_all();

    ledStatus ledStatusStruct;

    // init the I2C bus
    i2c_init(i2c_default, 100 * 1000); // 100khz
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    // setup button pin for increase.
    gpio_init(ROT_A);
    gpio_set_dir(ROT_A, GPIO_IN);

    // setup button pin for decrease.
    gpio_init(ROT_B);
    gpio_set_dir(ROT_B, GPIO_IN);

    // setup buttons
    for (int i = BUTTON1_PIN; i < BUTTON1_PIN + N_LED; i++)
    {
        gpio_init(i);
        gpio_set_dir(i, GPIO_IN);
        gpio_pull_up(i);
    }

    // #TODO: read from eeprom.

    // setup led(s).
    for (int i = STARTING_LED; i < STARTING_LED + N_LED; i++)
    {
        uint slice_num = pwm_gpio_to_slice_num(i);
        uint chan = pwm_gpio_to_channel(i);
        pwm_set_enabled(slice_num, false);
        pwm_config config = pwm_get_default_config();
        pwm_config_set_clkdiv_int(&config, 125);
        pwm_config_set_wrap(&config, 1000); // 1kHz
        pwm_init(slice_num, &config, false);
        // pwm_set_chan_level(slice_num, chan, ls.brightness); // brightness
        gpio_set_function(i, GPIO_FUNC_PWM);
        pwm_set_enabled(slice_num, true);
    }

    gpio_set_irq_enabled_with_callback(ROT_A, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(ROT_SW, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON1_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON2_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON3_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    queue_init(&irqEvents, sizeof(int), BUFFER_SIZE);

    int value = 0;
    int lastValue = 0;

    while (true)
    {
        while (queue_try_remove(&irqEvents, &value))
        {
            // breakout incase queue contains consecutive interrupts.
            if (value != lastValue && lastValue != 0)
            {
                break;
            }
            lastValue = value;
        }

        // handling interrupt events.
        // LED toggle buttons.
        if (value == BUTTON1_PIN || value == BUTTON2_PIN || value == BUTTON3_PIN)
        {
            toggleLED(value, &ledStatusStruct);
        }

        // RotA increaste brightness.
        if (value == ROT_A)
        {
            incBrightness(&ledStatusStruct);
        }

        // RotB decrease brightness.
        if (value == ROT_B)
        {
            decBrightness(&ledStatusStruct);
        }

        // Sanity check.
        if (value != BUTTON1_PIN && value != BUTTON2_PIN && value != BUTTON3_PIN && value != ROT_A && value != ROT_B)
        {
            printf("Unknown interrupt event: %d\n", value);
        }

        // Reset value.
        value = 0;
        lastValue = 0;
    }

    return 0;
}

static void gpio_callback(uint gpio, uint32_t event_mask)
{
    if (gpio == ROT_A)
    {
        if (gpio_get(ROT_B))
        {
            queue_try_add(&irqEvents, &ROT_B);
        }
    }
    queue_try_add(&irqEvents, &gpio);
    return;
}

void toggleLED(uint gpioPin, struct ledStatus *ledStatusStruct)
{
    printf("Toggle led pin %d\n", gpioPin);

    int ledNum = gpioPin - BUTTON1_PIN; // Results in 0, 1 or 2.
    ledStatusStruct->ledState[ledNum] = !ledStatusStruct->ledState[ledNum];

    int ledPin = gpioPin + 13; // 13 is the offset between the button and led pins.
    gpio_set_function(ledPin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(ledPin);
    pwm_set_enabled(slice_num, ledStatusStruct->ledState[ledNum]);
}

void incBrightness(struct ledStatus *ledStatusStruct)
{
    printf("Increase brightness\n");
    if (ledStatusStruct->brightness < LED_BRIGHT_MAX)
    {
        ledStatusStruct->brightness += LED_BRIGHT_STEP;
    }
}

void decBrightness(struct ledStatus *ledStatusStruct)
{
    printf("Decrease brightness\n");
    if (ledStatusStruct->brightness > LED_BRIGHT_MIN)
    {
        ledStatusStruct->brightness -= LED_BRIGHT_STEP;
    }
}
