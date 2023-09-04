#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int read_integer(void);
bool strtol_wrapper_b10(const char *s, long *n);

int main(void)
{
    int current_return = 0, ind = 0;
    double sum = 0;
    int return_array[128];

    current_return = read_integer();
    while (current_return >= 0)
    {
        return_array[ind] = current_return;
        sum += current_return;
        ind++;
        current_return = read_integer();
    }

    if (ind == 0)
    {
        printf("You entered no positive numbers. The average of 0 / 0  = a black hole.");
        return 0;
    }

    printf("You entered %d positive numbers. The average is: %.3f", ind, sum / (ind));
    return 0;
}


int read_integer(void)
{
    char input[10];
    int user_number_input;
    printf("Enter positive numbers or negative to stop: ");

    fgets(input, 10, stdin);
    strtol_wrapper_b10(&input, &user_number_input);

    // Invalid input loop.
    while (!strtol_wrapper_b10(&input, &user_number_input))
    {
        printf("Invalid input!\n");
        printf("Enter positive numbers or negative to stop: ");
        fgets(input, 10, stdin);
    }

    return user_number_input;
}


/* Not my code, code from https://c-for-dummies.com/blog/?p=3671.
Used it cause I found Atoi and strtol to be impossible to work with
and getting them to work properly required too much hassle to
check for correct input.*/
bool strtol_wrapper_b10(const char *s, long *n)
{
    char *p;

    *n = strtol(s, &p, 10);

    if (p == s)
        return false;
    else
        return true;
}