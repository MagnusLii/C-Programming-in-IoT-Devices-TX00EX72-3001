#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int read_range(int low, int high);
bool strtol_wrapper_b10(const char *s, long *n);

int main(void)
{
    int user_input = 0, computer_input = 0, round = 1, num_of_6 = 0;

    printf("Let's play!\n");
    while (round <= 3)
    {
        printf("Roll a die and enter your result.\n");
        user_input = read_range(1, 6);
        computer_input = user_input + 1;
        if (user_input == 6)
        {
            printf("I got %d. It is a tie!\n", user_input);
            num_of_6 += 1;
        }
        else
        {
            printf("I got %d. I win!\n", computer_input);
        }
        round++;
    }

    if (num_of_6 == 3)
    {
        printf("It's a draw. Cya.\n");
        return 0;
    }
    else
    {
        printf("Better luck next time. Bye!\n");
        return 0;
    }
}

int read_range(int low, int high)
{
    char input[10];
    int number = 420;
    bool valid_input = false;

    while (valid_input == false)
    {
        printf("Enter a number between %d and %d: ", low, high);
        fgets(input, 10, stdin);

        valid_input = strtol_wrapper_b10(&input, &number);

        if (number < 0 || number > high)  // strtol_wrapper_b10() returns 0 if no conversion is possible.
        {
            printf("Your input was outside the specified range.\n");
            valid_input = false;
        }
        else
        {
            printf("You entered something other than an integer.\n");
        }
    }

    return number;
}

bool strtol_wrapper_b10(const char *s, long *n)
{
    char *p;

    *n = strtol(s, &p, 10);

    if (p == s)
        return false;
    else
        return true;
}