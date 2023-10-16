#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct menu_item_
{
    char name[50];
    double price;
} menu_item;

#define INPUT_BUFFER_LENGHT 256
#define MENU_ITEM_SIZE 50

FILE *openFileWithRetry(const char *fileName, const char *mode, int maxRetries);
bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
int compareStrings(const void *a, const void *b);
void convertToLowercase(char *str);
int cmpfunc(const void *a, const void *b);
double storage = 0.0;

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
            strncpy(array[linesread].name, token, MENU_ITEM_SIZE);

            token = strtok(NULL, ";");
            if (token != NULL)
            {
                errno = 0;
                char *endptr = NULL;
                array[linesread].price = strtod(token, &endptr);
                if (errno == 0 && (*endptr == '\0' || *endptr == '\n'))
                {
                    linesread++;
                }
            }
        }
    }

    fclose(file);

    int choice = 0;
    do
    {
        printf("1. Sort by name\n");
        printf("2. Sort by price\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 2);

    int j = 0;
    switch (choice)
    {
    case 1:
        qsort(array, linesread, sizeof(menu_item), compareStrings);

        while (j < linesread)
        {
            printf("%8.2lf %s\n", array[j].price, array[j].name);
            j++;
        }
        break;

    case 2:
        qsort(array, linesread, sizeof(menu_item), cmpfunc);

        while (j < linesread)
        {
            printf("%8.2lf %s\n", array[j].price, array[j].name);
            j++;
        }
        break;

    default:
        printf("Hau are you in 'ere?\n");
        break;
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

int compareStrings(const void *a, const void *b)
{
    char aLower[INPUT_BUFFER_LENGHT] = "\0";
    char bLower[INPUT_BUFFER_LENGHT] = "\0";
    const menu_item *itemA = a;
    const menu_item *itemB = b;

    strncpy(aLower, itemA->name, MENU_ITEM_SIZE);
    aLower[MENU_ITEM_SIZE - 1] = '\0';
    strncpy(bLower, itemB->name, MENU_ITEM_SIZE);
    bLower[MENU_ITEM_SIZE - 1] = '\0';

    convertToLowercase(aLower);
    convertToLowercase(bLower);

    return strcmp(aLower, bLower);
}

int cmpfunc(const void *a, const void *b)
{
    const menu_item *itemA = a;
    const menu_item *itemB = b;

    if ((double)itemA->price > (double)itemB->price)
        return 1;
    else if ((double)itemA->price < (double)itemB->price)
        return -1;
    else
        return 0;
}

void convertToLowercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}