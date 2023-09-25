
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>


int main(void)
{

    int i = 0;
    char str[10] = "addd";
    i = strcmp(str, "add");

    printf("%d", i);

    return 0;
}