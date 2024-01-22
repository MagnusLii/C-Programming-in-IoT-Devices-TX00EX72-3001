#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "debug.h"
#include "debug.c"

#define INPUT_BUFFER_LENGHT 256

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
bool stringToIntConv(const char *str, int *result);

int main()
{
    srand(time(NULL));

    int debugLvl;
    char input[INPUT_BUFFER_LENGHT] = "\0";
    bool inputValid = false;

    do
    {
        printf("Enter debug level (0-4): ");
        improvedFgets(input, INPUT_BUFFER_LENGHT);
        inputValid = stringToIntConv(input, &debugLvl);
    } while (inputValid == false);

    if (debugLvl < 0 || debugLvl > 4)
    {
        printf("Error: not valid input\n");
        return 1;
    }

    set_debug_level(debugLvl);

    for (int i = 1; i <= 5; i++)
    {
        int rndDebugLvl = rand() % 5;
        dprintf(rndDebugLvl, "message has index %d\n", i);
    }

    return 0;
}

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString)
{
    bool newline_found = false; // Flag to track the presence of '\n'.
    int i = 0;

    if (fgets(stringToStoreTo, INPUT_BUFFER_LENGHT, stdin) != NULL)
    {
        // Checking for empty input.
        if (stringToStoreTo[0] == '\n' || stringToStoreTo[0] == '\0')
        {
            printf("Error: Empty input.\n");
            return false;
        }

        // Replacing \n with \0.
        while (newline_found == false)
        {
            if (stringToStoreTo[i] == '\n')
            {
                newline_found = true;
                stringToStoreTo[i] = '\0';
            }
            // Checking if input exceeds the maximum length.
            if (i >= maxLenghtOfString - 1)
            {
                printf("\nError: Input over max accepted lenght of %d characters.\n", maxLenghtOfString - 1);
                char dump[1000] = "\0";
                fgets(dump, 1000, stdin); // Clear excess input from stdin.
                return false;
            }
            i++;
        }
    }
    return true;
}

bool stringToIntConv(const char *str, int *result)
{
    char *endptr; // Pointer to the character after the converted part of the string.
    errno = 0;
    long int num = strtol(str, &endptr, 10);
    // Checking if input is beyond the range of a long integer.
    if (errno == ERANGE)
    {
        printf("Error: could not complete conversion to integer, number out of range.\nEnter a number between %d and %d.\n", LONG_MIN, LONG_MAX);
        return false;
    }
    // Checking if input contained anything other than numbers.
    else if (*endptr != '\0')
    {
        // Checking if input is non numerical.
        for (char *p = endptr; *p != '\0'; p++)
        {
            if (isdigit((unsigned char)*p) == false)
            {
                printf("Error: could not complete conversion to integer, you entered a non integer.\n");
                return false;
            }
        }
        printf("Error: could not read an integer.\n");
        return false;
    }
    *result = num;
    return true;
}