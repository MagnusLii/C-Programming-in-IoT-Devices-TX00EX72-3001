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
    int count = 0;
    count += printd("Characters: %c %c\n", 'a', 'A');
    count += printd("Decimals: %d %d\n", 1977, 650000);
    count += printd("Preceding with blanks: %10d \n", 1977);
    count += printd("Preceding with zeros: %010d \n", 1977);
    count += printd("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100, 100);
    count += printd("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
    count += printd("Width trick: %*d \n", 5, 10);
    count += printd("%s ", "A string");
    count += printd("Total number of chars: %d\n", count);


    printf("Count: %d\n", count);
    return 0;
}
