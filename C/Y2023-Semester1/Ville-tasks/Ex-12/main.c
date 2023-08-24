/*Write a function that receives an array of three integer pointers as a parameter.

The prototype of function is:

void sort3(int *pa[3]);

The function must sort the three pointers in the array so that the pointer to the smallest integer is the first element of the array and pointer to the largest integet is the last element of the array. The function may not modify the integer values.

For example if we have:

three integers: a, b, c

array of integer pointers int *pa[3]

and

a = 8, b = 2, c = 9 and pa[0] = &a, pa[1] = &b, pa[2] = &c

then after sort3(pa)

we have pa[0] = &b, pa[1] =&a, pa[2] = &c
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void sort3(int *pa[3]);
/* write a function that implements the function declared above */

#define TEST_SIZE 12

int main(int arcg, char **argv)
{
    const int v[TEST_SIZE] = {1, 2, 3, 7, 4, 9, 12, 4, -1, 67, 67, 34};
    int a[TEST_SIZE];
    int *ta[3];

    for (int i = 0; i < TEST_SIZE; i++)
        a[i] = v[i];

    for (int k = 0; k < TEST_SIZE; k += 3)
    {
        for (int i = 0; i < 3; ++i)
        {
            ta[i] = &a[i + k];
        }
        sort3(ta);

        //printf("%d, %d, %d\n", *ta[0], *ta[1], *ta[2]);
    }

    for (int i = 0; i < TEST_SIZE; ++i)
    {
        if (a[i] != v[i])
            printf("Data corrupted\n");
    }

    return 0;
}

void sort3(int *pa[3])
{
    int *temp;
    for (int i = 0; i < 3; i++)
    {
        for (int j = i + 1; j < 3; j++)
        {
            if (*pa[i] > *pa[j])
            {
                temp = pa[i];
                pa[i] = pa[j];
                pa[j] = temp;
            }
        }
    }
}