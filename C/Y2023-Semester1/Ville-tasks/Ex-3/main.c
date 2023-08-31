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