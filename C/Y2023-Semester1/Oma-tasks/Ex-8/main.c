#include <stdio.h>
#include <stdlib.h>

void print_numbers(const int *array, int count);

int main(void)
{
    int array[15] = {0};

    for (int i = 0; i < 15; i++)
    {
        array[i] = rand() % 100 + 1;
    }

    print_numbers(array, 15);

    return 0;
}

void print_numbers(const int *array, int count)
{
    int i = 0, j = 0;

    while (i < count)
    {
        printf("%2d ", array[i]);
        i++;
        j++;
        if (j == 8)
        {
            printf("\n");
            j = 0;
        }
    }
}