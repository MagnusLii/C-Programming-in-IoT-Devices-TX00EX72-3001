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