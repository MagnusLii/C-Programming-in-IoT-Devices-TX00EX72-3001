#include <stdio.h>
#include <stdbool.h>
#include <string.h>


// Function to read a binary number prefixed with "0b"
bool binary_reader(unsigned int *result) {
    char line[100]; // Adjust the buffer size as needed
    if (!read_line(line, sizeof(line))) {
        return false; // End of input
    }

    // Skip whitespace at the beginning of the line
    char *ptr = line;
    while (*ptr == ' ' || *ptr == '\t') {
        ptr++;
    }

    // Check for the "0b" prefix
    if (strncmp(ptr, "0b", 2) != 0) {
        return false; // Invalid format, missing "0b" prefix
    }

    ptr += 2; // Move past the "0b" prefix

    *result = 0; // Initialize the result to 0

    // Read binary digits and update the result
    while (*ptr == '0' || *ptr == '1') {
        *result <<= 1; // Shift left
        *result |= (*ptr - '0'); // Set the least significant bit
        ptr++;
    }

    return true; // Successfully read a binary number
}

// Function to count minimum hexadecimal digits needed to print a number
int digit_counter(unsigned int number) {
    int count = 0;
    while (number > 0) {
        number >>= 4; // Right shift by 4 bits (1 hexadecimal digit)
        count++;
    }
    return count;
}