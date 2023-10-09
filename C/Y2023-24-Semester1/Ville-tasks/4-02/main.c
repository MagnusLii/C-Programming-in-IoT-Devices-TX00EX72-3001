#include <stdio.h>
#include <string.h>
#include <ctype.h>

void merge_whitespace(char *);

void merge_whitespace(char *str)
{
    // replace space
    int i, j;
    for (i = 0; i < strlen(str); i++)
    {
        if (isspace(str[i]))
        {
            str[i] = ' ';
        }
    }

    // merge space
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ')
        {
            for (j = i; j < strlen(str); j++)
            {
                if (str[j] != ' ')
                {
                    break;
                }
            }
            if (j - i > 1)
            {
                memmove(&str[i + 1], &str[j], strlen(str) - j + 1);
            }
        }
    }


}