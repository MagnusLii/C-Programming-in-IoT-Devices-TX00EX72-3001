/*Write a function that receives two parameters and returns an integer. Parameters are an array and array size. Function finds the largest number in the array and returns it.*/

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

