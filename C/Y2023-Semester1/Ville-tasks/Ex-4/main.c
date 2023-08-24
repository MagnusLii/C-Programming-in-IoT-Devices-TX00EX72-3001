/*Write a function that prints ascending sequence of numbers. Function receives two integers as a parameter and prints a sequence of number starting from the lowest number and ending at the largest number. Program must print each number on a separate line and print an empty line at the end of the sequence.*/

#include <stdio.h>
#include <string.h>


void sequence(int num1, int num2)
{
    int i;
    if (num1 < num2)
    {
        for (i = num1; i <= num2; i++)
        {
            printf("%d\n", i);
        }
    }
    else
    {
        for (i = num2; i <= num1; i++)
        {
            printf("%d\n", i);
        }
    }
    printf("\n");
}