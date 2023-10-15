#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    char str[100];
    int i = 0;
    bool is_stop = false;

    while (is_stop == false)
    {
        printf("Enter a string: ");

        fgets(str, 10, stdin);
        size_t input_length = strlen(str);
        if (input_length > 0 && str[input_length - 1] == '\n')
        {
            str[input_length - 1] = '\0';
        }

        while (str[i] != '\0')
        {
            i++;
        }
        printf("The length of the string is: %d\n", i);

        if (strcmp(str, "stop") == 0)
        {
            is_stop = true;
        }
    }

    return 0;
}