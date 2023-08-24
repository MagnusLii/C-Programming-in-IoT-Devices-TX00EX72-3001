/*Write a program that reads positive numbers and calculates the average of read numbers. Program calls readint() to read an integer. Program stops reading if zero or negative number is entered. Program must print average value with 4 decimal accuracy.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readint(void);

int main(void)
{
    int num, sum = 0, count = 0;
    float avg;
    while (1)
    {
        num = readint();
        if (num <= 0)
        {
            break;
        }
        sum += num;
        count++;
    }
    avg = (float)sum / count;
    printf("%.4f\n", avg);

    return 0;
}