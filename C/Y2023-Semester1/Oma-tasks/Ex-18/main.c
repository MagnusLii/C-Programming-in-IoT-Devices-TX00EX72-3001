#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define INPUT_BUFFER_LENGTH 256

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
bool endToCancel(const char *inputStr);
void convertToLowercase(char *str);
bool stringToIntConv(const char *str, int *result);

int main()
{

    srand(time(NULL));
    bool end = false;
    char inputStr[INPUT_BUFFER_LENGTH];
    int inputint = 0;

    while (end == false)
    {
        printf("Enter a number in the range [0, 15] or a negative number to stop: ");
        if (improvedFgets(inputStr, INPUT_BUFFER_LENGTH) == true && stringToIntConv(inputStr, &inputint) == true && inputint >= 0 && inputint <= 15)
        {
            int randomNum = rand();
            printf("Random Number: %d\n", randomNum);
            printf("Random Hexadecimal Number: 0x%02X\n", randomNum);

            int shiftedNum = randomNum >> inputint;
            int result = shiftedNum & 0x3F;

            printf("Result in Hexadecimal: 0x%02X\n", result);
        }
        else if (inputint < 0)
        {
            end = true;
        }
        else
        {
            printf("Error: Enter an int less than 16.\n");
        }
    }

    printf("Exiting...\n");

    return 0;
}

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString)
{
    bool newline_found = false; // Flag to track the presence of '\n'.
    int i = 0;
    if (fgets(stringToStoreTo, maxLenghtOfString, stdin) != NULL)
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
                printf("\nError: Input over max accepted length of %d characters.\n", maxLenghtOfString - 1);
                return false;
            }
            i++;
        }
    }
    return true;
}

bool endToCancel(const char *inputStr)
{
    // Convert the input string to lowercase for case-insensitive comparison.
    char inputStrLower[INPUT_BUFFER_LENGTH] = "\0";
    int i = 0;
    strcpy(inputStrLower, inputStr);
    convertToLowercase(inputStrLower);

    if (strcmp(inputStrLower, "end") == 0)
    {
        printf("Cancelling...\n");
        return true; // Return true to indicate the "end" command was detected.
    }
    return false; // Return false if the input is not "end".
}

void convertToLowercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
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