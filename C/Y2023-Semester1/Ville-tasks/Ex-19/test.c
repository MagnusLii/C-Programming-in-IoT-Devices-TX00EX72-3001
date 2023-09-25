#include <stdio.h>

typedef void (*CharPrinter)(char);

int print_string(const char *str, CharPrinter printer) {
    printf("%s\n", str);
    return 0;
}

void custom_printer(char c) {
}

