#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Function to read a line from input
bool read_line(char *line, size_t max_len)
{
    if (fgets(line, max_len, stdin) == NULL)
    {
        return false; // End of input
    }
    return true;
}

// Function to perform calculations based on the command
void perform_calculation(int num1, int num2, char *command)
{
    if (strcmp(command, "add") == 0)
    {
        printf("%d + %d = %d\n", num1, num2, num1 + num2);
    }
    else if (strcmp(command, "sub") == 0)
    {
        printf("%d - %d = %d\n", num1, num2, num1 - num2);
    }
    else if (strcmp(command, "mul") == 0)
    {
        printf("%d * %d = %d\n", num1, num2, num1 * num2);
    }
    else if (strcmp(command, "div") == 0)
    {
        if (num2 != 0)
        {
            printf("%d / %d = %d\n", num1, num2, num1 / num2);
        }
        else
        {
            printf("Error: Division by zero\n");
        }
    }
    else
    {
        printf("Input: \"%d %d %s\" Unknown command: %s\n", num1, num2, command, command);
    }
}

int main()
{
    char line[100]; // Adjust the buffer size as needed

    while (read_line(line, sizeof(line)))
    {
        int num1 = 0, num2 = 0;
        char command[10]; // Assuming commands are no longer than 3 characters

        if (sscanf(line, "%d %d %s", &num1, &num2, command) == 3)
        {
            perform_calculation(num1, num2, command);
        }
        else
        {
            if (num1 == 0)
            {
                printf("Input: \"%s\" Invalid argument in position: %d\n", line, (num1 == 0) ? 1 : 2);
            }
            else if (num2 == 0)
            {
                printf("Input: \"%s\" Invalid argument in position: %d\n", line, (num1 == 0 && num2 == 0) ? 1 : 2);
            }
        }
    }

    return 0;
}
