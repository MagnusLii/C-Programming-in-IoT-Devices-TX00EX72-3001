#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "pico/util/queue.h"
#include <stdio.h>
#include <stdbool.h>

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

typedef struct ledStatus
{
    bool ledState[3];
    uint8_t brightness;
} ledStatus;

typedef struct queueTrain
{
    uint8_t gpioPin;
    uint8_t pinState;
} queueTrain;

static void gpio_callback(uint gpio, uint32_t event_mask);
void toggleLED(uint gpioPin, struct ledStatus *ledStatusStruct);
void incBrightness(struct ledStatus *ledStatusStruct);
void decBrightness(struct ledStatus *ledStatusStruct);
void buttonReleased(int gpioPin);
void writeLedStateToEeprom(const struct ledStatus *ledStatusStruct);
bool readLedStateFromEeprom(struct ledStatus *ledStatusStruct);

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
    ledStatusStruct.brightness = LED_BRIGHT_MIN;
    ledStatusStruct.ledState[0] = false;
    ledStatusStruct.ledState[1] = false;
    ledStatusStruct.ledState[2] = false;

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
        pwm_set_chan_level(slice_num, chan, 300); // brightness
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

            if (value == BUTTON1_PIN || value == BUTTON2_PIN || value == BUTTON3_PIN)
            {
                buttonReleased(value);
            }

            lastValue = value;
        }

        // handling interrupt events.
        // LED toggle buttons.
        if (lastValue == BUTTON1_PIN || lastValue == BUTTON2_PIN || lastValue == BUTTON3_PIN)
        {
            toggleLED(lastValue, &ledStatusStruct);
        }

        // RotA increaste brightness.
        if (lastValue == ROT_A)
        {
            incBrightness(&ledStatusStruct);
        }

        // RotB decrease brightness.
        if (lastValue == ROT_B)
        {
            decBrightness(&ledStatusStruct);
        }

        // Sanity check.
        if (lastValue != BUTTON1_PIN && lastValue != BUTTON2_PIN && lastValue != BUTTON3_PIN && lastValue != ROT_A && lastValue != ROT_B && lastValue != 0)
        {
            printf("Unknown interrupt event: %d\n", lastValue);
        }

        // Reset values.
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
            gpio = ROT_B;
        }
    }
    queue_try_add(&irqEvents, &gpio);
    return;
}

void toggleLED(uint gpioPin, struct ledStatus *ledStatusStruct)
{
    printf("Toggle led pin %d\n", gpioPin);

    int ledNum = gpioPin - BUTTON1_PIN; // Results in 0, 1 or 2.
    printf("ledNum: %d\n", ledNum);
    ledStatusStruct->ledState[ledNum] = !ledStatusStruct->ledState[ledNum];
    printf("ledState: %d\n", ledStatusStruct->ledState[ledNum]);

    int ledPin = gpioPin + 13; // 13 is the offset between the button and led pins.
    printf("ledPin: %d\n", ledPin);
    gpio_set_function(ledPin, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(ledPin);
    printf("slice_num: %d\n", slice_num);
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

void buttonReleased(int gpioPin)
{
    int debounceCounter = 0;

    while (debounceCounter < 100000)
    {
        if (gpio_get(gpioPin) == 1)
        {
            debounceCounter++;
        }
        else
        {
            debounceCounter = 0;
        }
    }

    return;
}

void writeLedStateToEeprom(const struct ledStatus *ledStatusStruct)
{
    uint16_t ledStatusAddress = LED_STATE_ADDR;
    uint16_t brightnessAddress = BRIGHTNESS_ADDR;

    // Write led state to eeprom.
    uint8_t statusAddr[2] = {ledStatusAddress >> 8, ledStatusAddress & 0xFF}; // High and low bytes of the EEPROM address
    uint8_t statusData[5] = {statusAddr[0], statusAddr[1], ledStatusStruct->ledState[0], ledStatusStruct->ledState[1], ledStatusStruct->ledState[2]};
    i2c_write_blocking(i2c_default, EEPROM_ADDR, statusData, 5, false);
    sleep_ms(EEPROM_WRITE_DELAY_MS);

    // Write inverse led state to eeprom.
    uint8_t inverseStatusAddr[2] = {(ledStatusAddress - 1) >> 8, (ledStatusAddress - 1) & 0xFF}; // High and low bytes of the EEPROM address for inverse LED states
    uint8_t inverseStatusData[5] = {inverseStatusAddr[0], inverseStatusAddr[1], !ledStatusStruct->ledState[0], !ledStatusStruct->ledState[1], !ledStatusStruct->ledState[2]};
    i2c_write_blocking(i2c_default, EEPROM_ADDR, inverseStatusData, 5, false);
    sleep_ms(EEPROM_WRITE_DELAY_MS);

    // Write brightness to eeprom.
    uint8_t brightnessAddr[2] = {brightnessAddress >> 8, brightnessAddress & 0xFF}; // High and low bytes of the EEPROM address
    uint8_t brightnessData[3] = {brightnessAddr[0], brightnessAddr[1], ledStatusStruct->brightness};
    i2c_write_blocking(i2c_default, EEPROM_ADDR, brightnessData, 3, false);
    sleep_ms(EEPROM_WRITE_DELAY_MS);
}

bool readLedStateFromEeprom(struct ledStatus *ledStatusStruct)
{
    uint16_t ledStatusAddress = LED_STATE_ADDR;
    uint16_t brightnessAddress = BRIGHTNESS_ADDR;
    uint8_t normalStatusData[5];
    uint8_t invertedStatusData[5];

    // Read led state from eeprom.
    uint8_t statusAddr[2] = {ledStatusAddress >> 8, ledStatusAddress & 0xFF}; // High and low bytes of the EEPROM address
    i2c_write_blocking(i2c_default, EEPROM_ADDR, statusAddr, 2, true);        // Write the register address with nostop=true
    uint8_t statusData[5];
    i2c_read_blocking(i2c_default, EEPROM_ADDR, statusData, 5, false);

    // Read inverted LED state data from EEPROM.
    uint8_t invertedStatusAddr[2] = {(ledStatusAddress - 1) >> 8, (ledStatusAddress - 1) & 0xFF};
    i2c_write_blocking(i2c_default, EEPROM_ADDR, invertedStatusAddr, 2, true);
    i2c_read_blocking(i2c_default, EEPROM_ADDR, invertedStatusData, 5, false);

    // Invert the inverted LED state data after reading.
    invertedStatusData[2] = ~invertedStatusData[2];
    invertedStatusData[3] = ~invertedStatusData[3];
    invertedStatusData[4] = ~invertedStatusData[4];

    // Compare the values.
    for (int i = 2; i < 5; ++i)
    {
        if (normalStatusData[i] != invertedStatusData[i])
        {
            printf("LED state mismatch\n");
            printf("normalStatusData: %d, %d, %d\n", normalStatusData[2], normalStatusData[3], normalStatusData[4]);
            printf("invertedStatusData: %d, %d, %d\n", invertedStatusData[2], invertedStatusData[3], invertedStatusData[4]);
            return false; // LED state mismatch
        }
    }

    // store the led state.
    ledStatusStruct->ledState[0] = normalStatusData[2];
    ledStatusStruct->ledState[1] = normalStatusData[3];
    ledStatusStruct->ledState[2] = normalStatusData[4];

    // Read brightness from eeprom.
    uint8_t brightnessAddr[2] = {brightnessAddress >> 8, brightnessAddress & 0xFF}; // High and low bytes of the EEPROM address
    i2c_write_blocking(i2c_default, EEPROM_ADDR, brightnessAddr, 2, true);           // Write the register address with nostop=true
    uint8_t brightnessData[3];
    i2c_read_blocking(i2c_default, EEPROM_ADDR, brightnessData, 3, false);
    ledStatusStruct->brightness = brightnessData[2];

    return true; // LED state matches
}