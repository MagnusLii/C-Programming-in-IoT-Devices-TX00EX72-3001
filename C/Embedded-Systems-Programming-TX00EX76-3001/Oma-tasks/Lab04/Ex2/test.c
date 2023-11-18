#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

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
#define BRIGHTNESS_ADDR 50
#define EEPROM_WRITE_DELAY_MS 5 
#define LED_STATE_ADDR 32768 // Address in the EEPROM to store the LED state
#define BUFFER_SIZE 512

typedef struct ledStatus
{
    bool ledState[3];
    uint16_t brightness;
} ledStatus;

void writeBrightnessToEeprom(const struct ledStatus *ledStatusStruct)
{
    printf("\nwriteBrightnessToEeprom\n");
    uint16_t brightnessAddress = BRIGHTNESS_ADDR;

    uint16_t brightnessDataByte = ledStatusStruct->brightness;

    uint8_t buffer[4];
    buffer[0] = (uint8_t)(brightnessAddress >> 8);
    buffer[1] = (uint8_t)(brightnessAddress & 0xFF);
    buffer[2] = (uint8_t)(brightnessDataByte >> 8);
    buffer[3] = (uint8_t)(brightnessDataByte & 0xFF);

    i2c_write_blocking(i2c_default, EEPROM_ADDR, buffer, 4, false);
    printf("buffer: %d %d %d %d\n", buffer[0], buffer[1], buffer[2], buffer[3]);
    sleep_ms(EEPROM_WRITE_DELAY_MS);
}

void readBrightnessFromEeprom(struct ledStatus *ledStatusStruct)
{
    printf("\nreadBrightnessFromEeprom\n");
    uint16_t brightnessAddress = BRIGHTNESS_ADDR;

    uint16_t brightnessDataByte;

    // Filling buffer with brightness address
    uint8_t buffer[2];
    buffer[0] = (uint8_t)(brightnessAddress >> 8);
    buffer[1] = (uint8_t)(brightnessAddress & 0xFF);

    i2c_write_blocking(i2c_default, EEPROM_ADDR, buffer, 2, true);
    sleep_ms(EEPROM_WRITE_DELAY_MS);
    i2c_read_blocking(i2c_default, EEPROM_ADDR, buffer, 2, false); // Fills buffer with brightness data

    brightnessDataByte = (buffer[0] << 8) | buffer[1];

    ledStatusStruct->brightness = (int)brightnessDataByte;
}