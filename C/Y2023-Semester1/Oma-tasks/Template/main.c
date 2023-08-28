#include <stdio.h>
#include <errno.h>

int read_integer(void);

int main(void)
{
    int current_return = 0, ind = 0;
    double sum = 0;
    int return_array[128];

    while (current_return >= 0)
    {
        current_return = read_integer();
        if (current_return < 0)
        {
            break;
        }
        else
        {
        return_array[ind] = current_return;
        sum += current_return;
        ind++;
        }
    }
    
    printf("You entered %d positive numbers. The average is: %.3f", ind - 1, sum / (ind - 1));
    
    return 0;
}

int read_integer(void)
{
    char input[10];
    int number;
    invalid_input:
    printf("Enter positive numbers or negative to stop: ");
    fgets(input, 10, stdin);

    errno = 0;
    number = strtol(input, NULL, 10);

    if (errno != 0)
    {
        printf("invalid input");
        goto invalid_input;
    }
    else
    {
        return number;
    }
}