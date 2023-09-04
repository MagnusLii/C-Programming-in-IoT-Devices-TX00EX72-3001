/*Ex7
Write a function named read_positive.
The function takes a pointer to int as a parameter and
returns a bool.

bool read_positive(int *value);

The function asks (=prints) user to enter a positive number.
If user enters a valid number and the
number is positive the function stores the number using the
pointer and returns true. If reading
number fails or number is not positive function returns
false without making changes to the value
using pointer.
Write a program that “plays” a guessing game with user.
The program asks user to guess how much
money it has and after the guess claims to twice as much
plus 20 euros. Program must use the
function above to ask user to enter the number. If reading
fails, function returns false, program asks
to enter a number again. Program stops when user has
entered an incorrect value three times.

Guess how much money I have!
Enter a positive number: 50
You didn’t get it right. I have 120 euros.
Guess how much money I have!
Enter a positive number: xd
Incorrect input
Guess how much money I have!
Enter a positive number: 70
You didn’t get it right. I have 160 euros.
Guess how much money I have!
Enter a positive number: fubar
Incorrect input
Guess how much money I have!
Enter a positive number: not
Incorrect input
I give up! See you later!*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <stdio.h>

int main(void)
{
    int user_input = 0, incorrect_guesses = 0;
    bool conversion_status = false;

    while (incorrect_guesses < 3)
    {
        printf("guess how much money I have!\n");
        printf("Enter a positive number: ");
        get_input(&user_input);
        conversion_status = read_positive(&user_input);

        if (conversion_status == false)
        {
            printf("Incorrect input\n");
            incorrect_guesses++;
        }
        else
        {
            printf("You didn’t get it right. I have %d euros.\n", (user_input * 2) + 20);
        }
    }

    printf("I give up! See you later!\n");

    return 0;
}

bool read_positive(int *value)
{
}

int get_input(int *intput_storage_location)
{
    char input[10];
    int number = 0;
    bool conversion_successful = false;

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
        printf("Incorrect input\n");
    }
    else if (*endptr != '\0') // In case of partial conversion, aka "3.6" -> "3".
    {
        printf("Incorrect input\n");
    }
    else
    {
        return number;
    }
}
