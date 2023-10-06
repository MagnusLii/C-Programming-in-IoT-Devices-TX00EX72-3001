#include <stdio.h>
#include <stdarg.h>

int printd(const char *format, ...) {
    va_list args;
    va_start(args, format);

    int result = printf("DEBUG: ");
    result += vprintf(format, args);

    va_end(args);
    return result;
}

int main() {
    int count = printd("test");
    printf("\nTotal characters: %d\n", count);
    return 0;
}
