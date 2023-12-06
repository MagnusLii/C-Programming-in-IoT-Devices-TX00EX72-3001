#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void get_input(char *input);
bool read_positive(int *value);

int main(void)
{
    int user_input = 0, incorrect_guesses = 0;
    bool conversion_status = false;

    while (incorrect_guesses < 3)
    {
        printf("guess how much money I have!\n");
        printf("Enter a positive number: ");
        conversion_status = read_positive(&user_input);

        if (conversion_status == false)
        {
            printf("Incorrect input\n");
            incorrect_guesses++;
        }
        else
        {
            printf("You didn\'t get it right. I have %d euros.\n", (user_input * 2) + 20);
        }
    }

    printf("I give up! See you later!\n");

    return 0;
}

bool read_positive(int *value)
{
    char input[10] = {0};
    int convertednumber = 0;

    get_input(input);

    char *endptr;
    convertednumber = strtol(input, &endptr, 10);

    if (input == endptr) // No number(s) found.
    {
        return false;
    }
    else if (*endptr != '\0') // In case of partial conversion, aka "3.6" -> "3".
    {
        return false;
    }
    else
    {
        *value = convertednumber;
        return true;
    }
}

void get_input(char *input)
{
    int convertednumber = 0;

    fgets(input, 10, stdin);
    size_t input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n')
    {
        input[input_length - 1] = '\0';
    }
}
