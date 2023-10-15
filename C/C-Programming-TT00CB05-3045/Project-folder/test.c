#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>

#define DB "db.txt"

bool fileReadLine(char *stringToStoreTo, int maxLenOfString, FILE *pointerToFileToReadFrom);

int main()
{

    char asd[100] = "asdasdasd";
    int size;

    size = sizeof(asd);
    size = strlen(asd);

    printf("%d\n", size);

    return 0;
}

bool fileReadLine(char *stringToStoreTo, int maxLenOfString, FILE *pointerToFileToReadFrom)
{
    char *buffer;
    int line_len = 0;
    buffer = (char *)malloc(line_len * sizeof(char));
    if (buffer == NULL)
    {
        fprintf(stdout, "Error: Unable to allocate buffer in fileReadLine.\n");
        free(buffer);
        return false;
    }

    if (fscanf(pointerToFileToReadFrom, "%s", buffer) != 1)
    {
        fprintf(stdout, "Error: Failed to read data from the file '%s'\n", DB);
        free(buffer);
        return false;
    }

    int indx = 0;
    bool strEnd = false;
    for (int i = 0; i < maxLenOfString; i++)
    {
        if (buffer[i] == '\0' || buffer[i] == '\n')
        {
            buffer[i] = '\0';
            strEnd = true;
        }
    }

    if (strEnd == false)
    {
        fprintf(stdout, "Error: The line in the file '%s' is too long.\n", DB);
        free(buffer);
        return false;
    }

    strncpy(stringToStoreTo, buffer, maxLenOfString);
    free(buffer);
    return true;
}