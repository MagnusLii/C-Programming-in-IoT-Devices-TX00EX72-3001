#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>

bool validateFormat(const char *input);

int main()
{

    FILE *pFile = fopen("db.txt", "r");

    char string[256] = {0};
    int readCount = fscanf(pFile, "%s", string);

    validateFormat(string);

    return 0;
}

bool validateFormat(const char *input)
{
    int val1, val2;
    if (sscanf(input, "%d %d", &val1, &val2) == 2)
    {
        printf("The input matches the expected format.\n");
        return true;
    }
    else
    {
        printf("The input does not match the expected format.\n");
        return false;
    }
}