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
#define BUFFER_SIZE 256

void send_command(const char* command);
void uart_rx_handler();
bool process_uart_data(const char expected_response[], int response_len);
bool process_DevEui_data(const char expected_response[], int response_len);

char circular_buffer[BUFFER_SIZE];
volatile int buffer_head = 0;
volatile int buffer_tail = 0;

int main() {
    stdio_init_all();

    gpio_init(SW_0_PIN);
    gpio_set_dir(SW_0_PIN, GPIO_IN);
    gpio_pull_up(SW_0_PIN);

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);

    // Set up the UART receive interrupt
    uart_set_irq_enables(UART_ID, true, false);
    irq_set_exclusive_handler(UART1_IRQ, uart_rx_handler);
    irq_set_enabled(UART1_IRQ, true);

    int state = 1;
    int attempts = 0;
    bool msg_status = false;

    while (true) {
        // Waiting for user to press SW_0
        if (state == 1) {
            printf("Press SW_0 to start communication with the LoRa module...\n");
            while (gpio_get(SW_0_PIN)) {
                sleep_ms(10);
            }
            state = 2;
        } 
        
        // Connecting to LoRa module
        else if (state == 2) {
            printf("Connecting to LoRa module...\n");
            send_command("AT\r\n");
            do
            {
                attempts++;
                msg_status = process_uart_data("+AT: OK", strlen("+AT: OK"));
                sleep_ms(TIMEOUT_MS);
            } while (attempts < 5 && msg_status == false);
            
            if (msg_status == true) {
                printf("Connected to LoRa module\n");
                state = 3;
            } else {
                printf("Module not responding\n");
                state = 1;
            }
            msg_status = false;
            attempts = 0;
        }   
        
        // Reading firmware version
        else if (state == 3) {
            printf("Reading firmware ver...\n");
            send_command("AT+VER\r\n");
            do
            {
                attempts++;
                msg_status = process_uart_data("+VER: ", strlen("+VER: "));
                sleep_ms(TIMEOUT_MS);
            } while (attempts < 5 && msg_status == false);


            if (msg_status == true) {
                state = 4;
            } else {
                printf("Module not responding\n");
                state = 1;
            }
        } 
        
        // Reading DevEui
        else if(state == 4) {
            printf("Reading DevEui...\n");
            send_command("AT+ID=DevEui\r\n");
            do
            {
                attempts++;
                msg_status = process_DevEui_data("+ID: DevEui,", strlen("+ID: DevEui,"));
                sleep_ms(TIMEOUT_MS);
            } while (attempts < 5 && msg_status == false);

            if (msg_status == false) {
                printf("Module not responding\n");
            }
            state = 1;
        }
    }
    return 0;
}

void send_command(const char* command) {
    uart_puts(UART_ID, command);
}

void uart_rx_handler() {
    while (uart_is_readable(UART_ID)) {
        char received_char = uart_getc(UART_ID);
        int next_head = (buffer_head + 1) % BUFFER_SIZE;

        if (next_head != buffer_tail) {
            circular_buffer[buffer_head] = received_char;
            buffer_head = next_head;
        } else {
            // Discard the data
        }
    }
}

bool process_uart_data(const char expected_response[], const int response_len) {
    int datalen = 0;
    char data[BUFFER_SIZE];

    // Check if there is data in the circular buffer
    while (buffer_tail != buffer_head) {
        data[datalen] = circular_buffer[buffer_tail];
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
        datalen++;
    }
    if (strncmp(data, expected_response, response_len) == 0){
        printf("received: %s\n", data);
        return true;
    }
    return false;
}

bool process_DevEui_data(const char expected_response[], const int response_len) {
    int datalen = 0;
    char data[BUFFER_SIZE];

    // Check if there is data in the circular buffer
    while (buffer_tail != buffer_head) {
        data[datalen] = circular_buffer[buffer_tail];
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
        datalen++;
    }

    if (strncmp(data, expected_response, response_len) == 0){
        for (int strlen = 0; strlen < datalen; strlen++){
            if (data[strlen] == ';'){
                for (int i = 0; i < strlen; i++){
                    data[i] = data[i + 1]; // remove ; from the string
                }
            } else {
                tolower(data[strlen]); // convert to lowercase
            }
        }

        // remove "+ID: DevEui, " from the string
        for (int i = 0; i < 12; i++){
            for (int j = 0; j < datalen; j++){
                data[j] = data[j + 1];
            }
        }
    }
    return false;
}
