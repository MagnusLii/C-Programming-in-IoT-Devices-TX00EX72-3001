#include <stdio.h>

typedef void (*CharPrinter)(char);

int print_string(const char *str, CharPrinter printer) {
    if (str == NULL) {
        return 0;
    }

    int charCount = 0;

    while (*str != '\0') {
        printer(*str);
        str++;
        charCount++;
    }

    return charCount;
}

void custom_printer(char c) {
    putchar(c);
}