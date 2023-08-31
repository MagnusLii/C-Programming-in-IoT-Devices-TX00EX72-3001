#include <stdio.h>
#include <string.h>


int largest(int *numbers, int count)
{
    int i, max = numbers[0];
    for (i = 1; i < count; i++)
    {
        if (numbers[i] > max)
        {
            max = numbers[i];
        }
    }
    return max;
}

