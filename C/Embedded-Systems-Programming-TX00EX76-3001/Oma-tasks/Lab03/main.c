#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define SW_0_PIN 9
#define TX_PIN 4
#define RX_PIN 5
#define UART_ID uart1
#define BAUD_RATE 9600
#define TIMEOUT_MS 500
#define STRLEN 128

void send_command(const char* command) {
    uart_puts(UART_ID, command);
}

bool read_response(const char expected_response, const char command, int max_attempts){
    bool expect_specific_response = true;
    char response[STRLEN];
    int pos = 0;

    if (strncmp("N/A", &expected_response, 3) == 0){
        printf("No response expected.");
        expect_specific_response = false;
    }

    for (int max_retries = 0; max_retries < max_attempts; max_retries++){
        printf("Sending command %s", &command)
        send_command(&command);
        sleep_ms(TIMEOUT_MS);
        while (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            if (c == '\r' || c == '\n') {
                response[pos] = '\0';
                printf("received: %s\n", response);
                pos = 0;  // Start over after the line is printed
                if (expect_specific_response == true /*&& strncmp(response, &expected_response, strlen(&expected_response)) == 0*/){
                    return true;
                }
            } else {
                if (pos < STRLEN - 1) {
                    response[pos++] = c;
                }
            }
        }
    }
    return false;
}

void read_dev_eui() {

    char response[STRLEN];
    int pos = 0;

    while (uart_is_readable(UART_ID)) {
        char c = uart_getc(UART_ID);
        if (c == '\r' || c == '\n') {
            response[pos] = '\0';
            printf("received: %s\n", response);
            pos = 0;  // Start over after the line is printed
            // Extract and format DevEui
            char* deveui_start = strstr(response, "+COMMAND: ");
            if (deveui_start != NULL) {
                deveui_start += 11;  // Move to the beginning of the DevEui value
                for (size_t i = 0; i < strlen(deveui_start); i++) {
                    if (deveui_start[i] != ' ') {
                        putchar(tolower(deveui_start[i]));
                    }
                }
            }
        } else {
            if (pos < STRLEN - 1) {
                response[pos++] = c;
            }
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(SW_0_PIN);
    gpio_set_dir(SW_0_PIN, GPIO_IN);
    gpio_pull_up(SW_0_PIN);

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

    int state = 1;

    sleep_ms(10000);

    while (1) {
        if (state == 1) {
            printf("Press SW_0 to start communication with the LoRa module...\n");
            while (gpio_get(SW_0_PIN)) {
                sleep_ms(10);
            }
            state = 2;
        } else if (state == 2) {
            printf("Connecting to LoRa module...\n");
            if (read_response("+ID: DevAddr,", "AT\r\n", 5)) {
                printf("Connected to LoRa module\n");
                state = 3;
                sleep_ms(10000);
            } else {
                printf("Module not responding\n");
                state = 1;
            }
        } else if (state == 3) {
            printf("Reading DevEui...\n");
            read_dev_eui();
            printf("\n");
            state = 1;
        }
    }

    return 0;
}
