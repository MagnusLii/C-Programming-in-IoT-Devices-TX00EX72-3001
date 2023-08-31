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