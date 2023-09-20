#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct menu_item_
{
    char name[50];
    double price;
} menu_item;

#define INPUT_BUFFER_LENGHT 256

FILE *openFileWithRetry(const char *fileName, const char *mode, int maxRetries);
bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);

int main(void)
{
    bool test = false;
    char filename[INPUT_BUFFER_LENGHT] = "\0";
    FILE *file = NULL;
    menu_item array[40];

    do
    {
        printf("Enter a filename: ");
        test = improvedFgets(filename, INPUT_BUFFER_LENGHT);
    } while (test == false);

    file = openFileWithRetry(filename, "r", 3);

    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 0;
    }

    int linesread = 0;
    char line[INPUT_BUFFER_LENGHT];
    while (linesread < 40 && fgets(line, sizeof(line), file) != NULL)
    {

        char *token = strtok(line, ";");
        if (token != NULL)
        {

            while (isspace(*token))
            {
                token++;
            }
            strcpy(array[linesread].name, token);

            token = strtok(NULL, ";");
            if (token != NULL)
            {
                array[linesread].price = atof(token);
                linesread++;
            }
        }
    }

    fclose(file);

    int j = 0;
    while (j < linesread)
    {
        printf("%8.2lf %s\n", array[j].price, array[j].name);
        j++;
    }

    return 0;
}

FILE *openFileWithRetry(const char *fileName, const char *mode, int maxRetries)
{
    FILE *file = NULL;
    int retryCount = 0;

    while (retryCount < maxRetries)
    {
        file = fopen(fileName, mode);
        if (file != NULL)
        {
            return file;
        }

        retryCount++;
    }

    return NULL;
}

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString)
{
    bool newline_found = false;
    int i = 0;

    if (fgets(stringToStoreTo, INPUT_BUFFER_LENGHT, stdin) != NULL)
    {

        if (stringToStoreTo[0] == '\n' || stringToStoreTo[0] == '\0')
        {
            printf("Error: Empty input.\n");
            stringToStoreTo[0] = '\0'; // Clearing string.
            return false;
        }

        while (newline_found == false)
        {
            if (stringToStoreTo[i] == '\n')
            {
                newline_found = true;
                stringToStoreTo[i] = '\0';
            }
            if (i >= maxLenghtOfString - 1)
            {
                printf("\nError: Input over max accepted lenght of %d characters.\n", maxLenghtOfString - 1);
                stringToStoreTo[0] = '\0'; // Clearing string.
                return false;
            }
            i++;
        }
    }

    return true;
}