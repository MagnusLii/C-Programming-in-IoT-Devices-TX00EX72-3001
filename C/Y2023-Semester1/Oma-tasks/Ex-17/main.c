#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define INPUT_BUFFER_LENGHT 32
#define MAX_PASSWORD_LENGHT INPUT_BUFFER_LENGHT * 2 + 1

bool generatePassword(char *password, int size, const char *word);
bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
bool exitToCancel(const char *inputStr);
void convertToLowercase(char *str);

int main()
{
    srand(time(NULL));

    char input[INPUT_BUFFER_LENGHT];
    char password[(INPUT_BUFFER_LENGHT * 2) + 1];
    bool stop = false, inputLen = false;

    while (stop == false)
    {
        printf("Enter base to generate password from: ");
        inputLen = improvedFgets(input, INPUT_BUFFER_LENGHT);

        if (exitToCancel(input) == true)
        {
            stop = true;
        }
        else if (generatePassword(password, MAX_PASSWORD_LENGHT, input) == true && inputLen == true)
        {
            printf("Generated password: %s\n", password);
        }
        else
        {
            printf("Error: incorrect input.\n");
        }
    }

    return 0;
}

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString)
{
    bool newline_found = false; // Flag to track the presence of '\n'.
    int i = 0;
    char dump[1000] = "\0";
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
                fgets(dump, 1000, stdin);
                return false;
            }
            i++;
        }
    }
    return true;
}

bool generatePassword(char *password, const int size, const char *input)
{
    if (size < ((strlen(input) * 2) + 1))
    {
        printf("Error: your input is too long.\n");
        return false;
    }

    const char printable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";

    int wordLen = strlen(input);
    int passwordLen = wordLen * 2 + 1;

    password[0] = printable[rand() % (sizeof(printable) - 1)];
    password[passwordLen - 1] = printable[rand() % (sizeof(printable) - 1)];

    for (int i = 1; i < passwordLen - 1; i += 2)
    {
        password[i] = input[i / 2];
        password[i + 1] = printable[rand() % (sizeof(printable) - 1)];
    }

    password[passwordLen] = '\0';

    return true;
}

bool exitToCancel(const char *inputStr)
{
    // Convert the input string to lowercase for case-insensitive comparison.
    char inputStrLower[INPUT_BUFFER_LENGHT] = "\0";
    int i = 0;

    strcpy(inputStrLower, inputStr);
    convertToLowercase(inputStrLower);

    if (strcmp(inputStrLower, "stop") == 0)
    {
        printf("Cancelling...\n");
        return true; // Return true to indicate the "exit" command was detected.
    }
    return false; // Return false if the input is not "exit".
}

void convertToLowercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}