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
    uart_putc(UART_ID, '\n'); // End each command with '\n'
}

bool read_response(const char* expected_response, int max_attempts) {
    char response[STRLEN];
    int pos = 0;
    for (int i = 0; i < max_attempts; i++) {
        send_command("AT+COMMAND?"); // Query
        sleep_ms(TIMEOUT_MS);
        while (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);
            if (c == '\r' || c == '\n') {
                response[pos] = '\0';
                printf("received: %s\n", response);
                pos = 0;  // Start over after the line is printed
                if (strstr(response, expected_response) != NULL) {
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
    send_command("AT+COMMAND=DATA"); // Set value format
    sleep_ms(TIMEOUT_MS);
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

    printf("LoRa module test\n");
    sleep_ms(1000);

    while (1) {
        if (state == 1) {
            printf("Press SW_0 to start communication with the LoRa module...\n");
            while (gpio_get(SW_0_PIN)) {
                sleep_ms(10);
            }
                        state = 2;
        } else if (state == 2) {
            printf("Connecting to LoRa module...\n");
            send_command("+AT: OK\r\n");
            if (read_response("OK", 5)) {
                printf("Connected to LoRa module\n");
                state = 3;
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
