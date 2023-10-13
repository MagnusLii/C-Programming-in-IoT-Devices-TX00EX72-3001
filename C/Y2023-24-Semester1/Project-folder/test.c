#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>


bool fileRead(char *stringToStoreTo, FILE *fileToReadFrom);

int main()
{

    FILE *pFile = fopen("db.txt", "r");
    char string[256] = {0};

    while (fileRead(string ,pFile) != false)
    {
        printf("%s", string);
    }
    
    return 0;
}

bool fileRead(char *stringToStoreTo, FILE *fileToReadFrom){
    char *buffer;
    size_t line_len = 0;
    ssize_t bufferLen;
    buffer = (char *)malloc(line_len * sizeof(char));
    if( buffer == NULL){
        fprintf(stdout, "Error: Unable to allocate buffer in fileRead.\n");
        free(buffer);
        return false;
    }

    bufferLen = getline(&buffer, &line_len, fileToReadFrom);
    if (bufferLen == -1 || bufferLen == 0){
        fprintf(stdout, "Error: Failed to read data from the file '%s'\n", "db.txt");
        free(buffer);
        return false;
    }
    else if (bufferLen > 256){
        fprintf(stdout, "Error: Line length exceeds buffer length in fileRead.\n");
        free(buffer);
        return false;
    }

    strncpy(stringToStoreTo, buffer, sizeof(stringToStoreTo) - 1);
    stringToStoreTo[sizeof(stringToStoreTo) - 1] = '\0';
    free(buffer);

    return true;
}