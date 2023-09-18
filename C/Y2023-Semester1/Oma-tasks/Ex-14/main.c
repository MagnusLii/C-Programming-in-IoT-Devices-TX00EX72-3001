/*Write a program that reads lines of text in to an array of strings. The maximum length of the line is
80 characters and the maximum number of lines is 100.
The program asks user to enter a filename. Then program tries to open the file in textmode for
reading. If opening the file fails the program prints an error message with the filename to stderr and
then exits. The program reads lines from the file into the array until the file ends or the array is full
(100 lines read). Then program closes the file.
Then program converts all letters to upper case (see toupper() – function).
Then program opens the file for writing in text mode and writes all read lines to file and closes the
file.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define INPUT_BUFFER_LENGHT 256

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
FILE *openFileWithRetry(const char *fileName, const char *mode, int maxRetries);

int main(void)
{

    bool test = false;
    char array[100][80];
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

    int linecount = 0;
    while (fgets(array[linecount], 80, file) != NULL && linecount < 100)
    {
        for (int i = 0; i < strlen(array[linecount]); i++)
        {
            array[linecount][i] = toupper(array[linecount][i]);
        }
        linecount++;
    }
    fclose(file);

    file = openFileWithRetry(filename, "w", 3);

    for (int i = 0; i < linecount; i++)
    {
        fprintf(file, "%s", array[i]);
    }
    
    fclose(file);

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