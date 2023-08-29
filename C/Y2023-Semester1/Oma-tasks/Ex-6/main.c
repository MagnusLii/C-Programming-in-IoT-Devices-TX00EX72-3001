#include <stdio.h>
#include <stdlib.h>

int read_range(int low, int high);

int main(void)
{
    int user_input = 0;
    char input[10];

    do
    {
        printf("\n--------------------------------\n"
               "1. Roll D6\n"
               "2. Roll D10\n"
               "3. Quit\n");

        user_input = read_range(1, 3);

        if (user_input == 1)
        {
            printf("You rolled a %d.\n", rand() % 6 + 1);
        }
        else if (user_input == 2)
        {
            printf("You rolled a %d.\n", rand() % 10 + 1);
        }

    } while (user_input != 3);

    printf("Bye!\n");

    return 0;
}

int read_range(int low, int high)
{
    char input[10];
    int number = 0;
invalid_input:

    printf("Enter a number between %d and %d: ", low, high);

    fgets(input, 10, stdin);

    size_t input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n')
    {
        input[input_length - 1] = '\0';
    }

    char *endptr;

    number = strtol(input, &endptr, 10);

    if (input == endptr) // No number(s) found.
    {
        printf("You entered something other than an integer.\n");
        goto invalid_input;
    }
    else if (*endptr != '\0') // In case of partial conversion, aka "3.6" -> "3".
    {
        printf("You entered something other than an integer !!!!.\n");
        goto invalid_input;
    }
    else if (number < low || number > high)
    {
        printf("Your input was outside the specified range.\n");
        goto invalid_input;
    }
    else
    {
        return number;
    }
}
