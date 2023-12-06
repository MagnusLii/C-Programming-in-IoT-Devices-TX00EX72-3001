#include <stdio.h>
#include <stdbool.h>

#define INPUT_BUFFER_LENGHT 256

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
FILE *openFileWithRetry(const char *fileName, const char *mode, int maxRetries);

int main(void)
{
    bool test = false;
    char filename[INPUT_BUFFER_LENGHT] = "\0";
    FILE *file = NULL;

    printf("Enter a filename: ");
    do
    {
        test = improvedFgets(filename, INPUT_BUFFER_LENGHT);
    } while (test == false);

    file = openFileWithRetry(filename, "r", 3);

    if (file == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 0;
    }

    int num = 0, min = 0, max = 0, count = 0;

    while (fscanf(file, "%d", &num) == 1)
    {
        if (count == 0)
        {
            min = num;
            max = num;
        }
        else
        {
            if (num < min)
            {
                min = num;
            }
            if (num > max)
            {
                max = num;
            }
        }
        count++;
    }

    printf("Count of numbers: %d\n", count);
    printf("Lowest number: %d\n", min);
    printf("Highest number: %d\n", max);

    return 0;
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