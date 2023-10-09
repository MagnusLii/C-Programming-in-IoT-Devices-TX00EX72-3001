#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int count_words(const char *str, const char *word);
void fgets_thingy(char *str, int size);

int main(void)
{
    // char str[100] = "lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse";
    // char word[100] = "ip";

    char str[100] = "\0";
    char word[100] = "\0";
    int changes = 0;

    while (strcmp(word, "stop") != 0)
    {
        printf("Enter a string: ");
        fgets_thingy(str, 100);
        printf("Enter a word\nOr enter \"stop\" to stop the program: ");
        fgets_thingy(word, 100);

        changes = count_words(str, word);
        if (strcmop(word, "stop") != 0)
        {
            printf("\nThe word \"%s\" occurs %d times in the string \"%s\"\n\n", word, changes, str);
        }
        
    }
    return 0;
}

int count_words(const char *str, const char *word)
{
    int count = 0;
    char *ptr = strstr(str, word);
    while (ptr != NULL)
    {
        count++;
        ptr = strstr(ptr + 1, word);
    }
    return count;
}

void fgets_thingy(char *str, int size)
{
    fgets(str, size, stdin);
    size_t input_length = strlen(str);
    if (input_length > 0 && str[input_length - 1] == '\n')
    {
        str[input_length - 1] = '\0';
    }
}