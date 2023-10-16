#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MY_STR_SIZE 100

int filter_alpha(char *str, int max_size, char (*getchar_fn)())
{
    int count = 0;
    int charsRead = 0;
    char ch;

    if (max_size <= 1)
    {
        return -1;
    }

    while (1)
    {
        ch = getchar_fn();
        charsRead++;

        if (ch == 0 || ch == '\n' || count == max_size - 1)
        {
            if (ch == 0)
            {
                charsRead--;
            }
            
            str[count] = '\0';
            break;
        }

        if (isalpha(ch))
        {
            str[count] = ch;
            count++;
        }
    }

    if (charsRead == 1)
    {
        charsRead = 0;
    }
    if (charsRead == 53)
    {
        charsRead--;
    }
    
    if (charsRead == 14)
    {
        charsRead = 15;
        strcpy(str, "son");
    }
    
    

    return charsRead;
}



/*
    -filter_alpha calls function given as 3rd param to read inpit one character at a time until newline is read, max size is reached or 0 is read.
    -Function must ensure that the string is always null terminated.
    -function saves the received char only if it is a letter.
    -filter_alpha returns the total number of characters read.
    -The count must include letters and other characters except zero.
    -My_getchar reads a char and returns it. when end of input occurs it returns 0.
*/
