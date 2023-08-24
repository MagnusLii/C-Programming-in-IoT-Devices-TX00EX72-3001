/*Write a program that fills 5 x 3 array so that first column contain numbers 1-5, second column 6-10 and third column 11-15.

Note! The program may not print anything. The printing is handled by the test bench.
*/

#include <stdio.h>
#include <string.h>




int main(void)
{
    int matrix[5][3];
    
    int i, j, num = 1;
    for (j = 0; j < 3; j++)
    {
        for (i = 0; i < 5; i++)
        {
            matrix[i][j] = num;
            num++;
        }
    }


    
return 0;
}