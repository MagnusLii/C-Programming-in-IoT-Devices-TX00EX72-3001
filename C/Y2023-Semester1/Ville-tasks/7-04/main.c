#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

/*
    -count_args takes an integer pointer and variable number of other arguments as parameters and returns an integer.
    -The variabale arguments are pointers to strings.
    -The end of arguments is indicated by a NULL pointer.
    -The function returns the number of strings.
    -Function also counts total number of characters in the strings and stores the total through the first parameter.
*/


int count_args(int *total_chars, ...) {
    va_list args;
    char *str;
    int count = 0;

    *total_chars = 0;

    va_start(args, total_chars);
    while ((str = va_arg(args, char*)) != NULL) {
        count++;
        *total_chars += strlen(str);
    }
    va_end(args);

    return count;
}
