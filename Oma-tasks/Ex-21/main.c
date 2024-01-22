#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define INPUT_BUFFER_LENGHT 256

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
FILE *openFileWithRetry(const char *fileName, const char *mode, const int maxRetries);
unsigned char calculateChecksum(const char *line);

int main()
{
    char filename[INPUT_BUFFER_LENGHT];
    bool inputValid = false;

    do
    {
        printf("Enter the file name: ");
        inputValid = improvedFgets(filename, INPUT_BUFFER_LENGHT);
    } while (inputValid == false);

    FILE *file = openFileWithRetry(filename, "r", 3);
    if (file == NULL)
    {
        return 1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file))
    {
        if (buffer[0] == '$')
        {
            char *asterisk = strchr(buffer, '*');
            if (asterisk != NULL)
            {
                unsigned int expected_checksum;
                if (sscanf(asterisk + 1, "%2X", &expected_checksum) == 1)
                {
                    unsigned char actual_checksum = calculateChecksum(buffer);

                    if (actual_checksum == expected_checksum)
                    {
                        printf("[ OK ] %s", buffer);
                    }
                    else
                    {
                        printf("[FAIL] %s", buffer);
                    }
                }
                else
                {
                    printf("[FAIL] %s", buffer);
                }
            }
        }
    }

    fclose(file);
    return 0;
}

bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString)
{
    bool newline_found = false; // Flag to track the presence of '\n'.
    int i = 0;

    if (fgets(stringToStoreTo, INPUT_BUFFER_LENGHT, stdin) != NULL)
    {
        // Checking for empty input.
        if (stringToStoreTo[0] == '\n' || stringToStoreTo[0] == '\0')
        {
            printf("Error: Empty input.\n");
            return false;
        }

        // Replacing \n with \0.
        while (newline_found == false)
        {
            if (stringToStoreTo[i] == '\n')
            {
                newline_found = true;
                stringToStoreTo[i] = '\0';
            }
            // Checking if input exceeds the maximum length.
            if (i >= maxLenghtOfString - 1)
            {
                printf("\nError: Input over max accepted lenght of %d characters.\n", maxLenghtOfString - 1);
                char dump[1000] = "\0";
                fgets(dump, 1000, stdin); // Clear excess input from stdin.
                return false;
            }
            i++;
        }
    }
    return true;
}

FILE *openFileWithRetry(const char *fileName, const char *mode, const int maxRetries)
{
    FILE *file = NULL;
    int retryCount = 0;

    // Attempt to open the file 'maxRetries' times.
    while (retryCount < maxRetries)
    {
        file = fopen(fileName, mode);
        if (file != NULL)
        {
            return file; // File opened successfully.
        }
        retryCount++;
    }

    // Unable to open the file after maximum retries.
    printf("Error: Unable to open the file \"%s\".\n", fileName);
    return NULL;
}

unsigned char calculateChecksum(const char *line)
{
    unsigned char checksum = 0;
    int i;

    // Start at the character after '$' and iterate until just before '*'
    for (i = 1; line[i] != '*' && line[i] != '\0'; i++)
    {
        checksum ^= line[i];
    }

    return checksum;
}