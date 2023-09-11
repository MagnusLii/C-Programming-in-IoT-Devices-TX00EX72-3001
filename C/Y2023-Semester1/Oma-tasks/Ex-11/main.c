#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int replace_char(char *str, const char *repl);
void fgets_thingy(char *str, int size);

int main(void)
{
    //char text[80] = "I am so tired of Python. C is much better language";
    int count = 0;
    char text[80], replace[3];
    size_t size;
    
    printf("Enter a string: ");
    fgets_thingy(text, 80);

    printf("Enter a string containing two characters: ");
    fgets_thingy(replace, 3);

    //count = replace_char(text, "e3");

    printf("\n\n");
    printf("The number of characters replaced is: %d\n", count);
    printf("The modified string is: %s\n", text);
    return 0;
}

int replace_char(char *str, const char *repl)
{
    int i = 0, count = 0;

    while (str[i] != '\0')
    {
        if (str[i] == repl[0])
        {
            str[i] = repl[1];
            count++;
        }
        i++;
    }

    return count;
}

void fgets_thingy(char *str, int size)
{
    fgets(str, size, stdin);
    printf("%s", str);
    size_t input_length = strlen(str);
    if (input_length > 0 && str[input_length - 1] == '\n')
    {
        str[input_length - 1] = '\0';
    }
    
}