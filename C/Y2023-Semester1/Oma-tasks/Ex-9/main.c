#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void get_input(int *input);
int find_first(const unsigned int *array, unsigned int what);

int main(void)
{
    int array[20] = {0};
    int user_input = 0;

    for (int i = 0; i < 19; i++)
    {
        array[i] = rand() % 9 + 1;
    }
    array[19] = 0;

    printf("Enter number to search for or zero (\"0\") to stop: ");
    get_input(&user_input);

    while (user_input != 0)
    {

        int index = find_first(array, user_input);
        if (index == -1)
        {
            printf("Not found.\n");
        }
        else
        {
            printf("Found at index %d.\n", index);
        }
        printf("Enter number to search for or zero (\"0\") to stop: ");
        get_input(&user_input);
    }

    return 0;
}

int find_first(const unsigned int *array, unsigned int what)
{
    int i = 0;
    while (array[i] != 0)
    {
        if (array[i] == what)
        {
            return i;
        }
        i++;
    }

    return -1;
}

void get_input(int *storage)
{
    int convertednumber = 0;
    char input[10] = {0};

    fgets(input, 10, stdin);
    size_t input_length = strlen(input);
    if (input_length > 0 && input[input_length - 1] == '\n')
    {
        input[input_length - 1] = '\0';
    }

    char *endptr;
    convertednumber = strtol(input, &endptr, 10);

    if (input == endptr) // No number(s) found.
    {
        printf("Haha you're funny :)\n");
    }
    else if (*endptr != '\0') // In case of partial conversion, aka "3.6" -> "3".
    {
        printf("The hell is wrong with you?!= enter a number.\n");
    }
    else
    {
        *storage = convertednumber;
    }
}