#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool binary_reader(unsigned int *result) {
    char line[100];
    if (!read_line(line, sizeof(line))) {
        return false;
    }

    char *ptr = line;
    while (*ptr == ' ' || *ptr == '\t') {
        ptr++;
    }


    if (strncmp(ptr, "0b", 2) != 0) {
        return false;
    }

    ptr += 2;

    *result = 0;


    while (*ptr == '0' || *ptr == '1') {
        *result <<= 1;
        *result |= (*ptr - '0');
        ptr++;
    }

    return true;
}


int digit_counter(unsigned int number) {
    int count = 0;
    while (number > 0) {
        number >>= 4;
        count++;
    }
    if (count == 0) {
        return 1;
    }
    return count;
}