#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

void print_binaryw(uint32_t value, uint32_t width)
{
    //printf("value: %d", value);
    //printf("   width: %d\n", width);
    char binary[33];
    int index = 32;
    int firstOneLoc = 33, firstOneMath = 0;

    binary[index--] = '\0'; // Null-terminate the string

    do {
        binary[index--] = (value & 1) ? '1' : '0'; // Convert each bit to a character
        value >>= 1; // Shift the value to the right
    } while (value);

    // Fill the rest of the string with leading zeros
    while (index >= 0) {
        binary[index--] = '0';
    }

    // find first 1
    for (int i = 0; i < 33; i++)
    {
        if (binary[i] == '1')
        {
            firstOneLoc = i;
            firstOneMath = 33 - i;
            break;
        }
    }
    

    // If the width is less than the length of the binary string, print the entire string
    if (width > strlen(binary)) {
        //printf("if\n");
        printf("%s", binary);
    }

    if (firstOneMath > width)
    {
        //printf("if2\n");
        printf("%s", &binary[firstOneLoc]);
    }
    

    // Otherwise, print only the last 'width' characters
    else {
        //printf("else\n");
        printf("%s", &binary[strlen(binary) - width]);
    }
}


/*
    -Implement a function that takes two parameters: a number to print and field width.
    -The function prints the number in binary with the specified field width filling the field with leading zeros if necessary.
    -If printing requires more space than the field width specifies the number is still printed in full.
    -The function must not print any white space before or after the number.
    -For example:   print_binaryw(5, 4) prints 0101
                    print_binaryw(0x27, 4) prints 100111
                    print_binaryw(5, 0) prints 101
*/