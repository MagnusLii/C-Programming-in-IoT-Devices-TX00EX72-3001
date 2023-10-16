
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// bool read_line(char *line, size_t size);
int format_tester(const char *input, int size_of_string, int *pnum1, int *pnum2, char *poperator);

int main(void)
{
    int num1 = 0, num2 = 0;
    char operator[4] = {0};

    int *pnum1 = &num1, *pnum2 = &num2;
    char *poperator = operator;

    char input[255] = "7 2 add";
    size_t size = sizeof(input);
    char *pinput = input;

    bool status = false;

    if (true == true)
    {
        format_tester(pinput, size, pnum1, pnum2, poperator);
        printf("%d %s %d\n", pnum1, poperator, pnum2);
    }

    return 0;
}

int format_tester(const char *input, int size_of_string, int *pnum1, int *pnum2, char *poperator)
{
    int test1, test2;
    char test3[4] = {0};
    char input2[255] = "7 2 add";
    if (sscanf(input2, "%d %d %s", test1, test2, test3) == 3)
    {
        *pnum1 = test1;
        *pnum2 = test2;
        poperator = test3;
        return 0;
    }
}

/*
int error_finder(const char *input, int size_of_string, int *pnum1, int *pnum2, char *poperator)
{
    // the fuck do I do here now...................
    char string[256] = *input;
    int i = 0, num_of_spaces = 0;

    while (string[i] != '\0')
    {
        switch (num_of_spaces)
        {
        case 0:
            if (string[i] !=)
            {

            }

            break;

        default:
            break;
        }
    }
}

*/