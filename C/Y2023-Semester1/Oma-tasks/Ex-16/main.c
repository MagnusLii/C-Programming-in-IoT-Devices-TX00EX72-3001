#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define INPUT_BUFFER_LENGHT 256

typedef struct node
{
    int number;
    struct node *next;
} nnode;

bool stringToIntConv(const char *str, int *result);
bool endToCancel(const char *inputStr);
bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
void convertToLowercase(char *str);
nnode *createNode(int num);
void insertNode(nnode **head, int num);
void printList(nnode *head);
void freeList(nnode *head);


int main()
{
    nnode *head = NULL;
    char input[INPUT_BUFFER_LENGHT];
    bool end = false;
    bool inputValid = false;

    while (end == false)
    {
        printf("Enter a number or 'end' to stop: ");
        inputValid = improvedFgets(input, INPUT_BUFFER_LENGHT);

        if (inputValid == true)
        {
            if (endToCancel(input) == true)
            {
                end = true;
            }
            else
            {
                int num;
                if (stringToIntConv(input, &num) == true)
                {
                    insertNode(&head, num);
                }
            }
        }
    }

    printf("Entered numbers: ");
    printList(head);
    freeList(head);

    return 0;
}

bool stringToIntConv(const char *str, int *result)
{
    char *endptr; // Pointer to the character after the converted part of the string.
    errno = 0;
    long int num = strtol(str, &endptr, 10);
    // Checking if input is beyond the range of a long integer.
    if (errno == ERANGE)
    {
        printf("Error: could not complete conversion to integer, number out of range.\nEnter a number between %d and %d.\n", LONG_MIN, LONG_MAX);
        return false;
    }
    // Checking if input contained anything other than numbers.
    else if (*endptr != '\0')
    {
        // Checking if input is non numerical.
        for (char *p = endptr; *p != '\0'; p++)
        {
            if (isdigit((unsigned char)*p) == false)
            {
                printf("Error: could not complete conversion to integer, you entered a non integer.\n");
                return false;
            }
        }
        printf("Error: could not read an integer.\n");
        return false;
    }
    *result = num;
    return true;
}

bool endToCancel(const char *inputStr)
{
    // Convert the input string to lowercase for case-insensitive comparison.
    char inputStrLower[INPUT_BUFFER_LENGHT] = "\0";
    int i = 0;
    strcpy(inputStrLower, inputStr);
    convertToLowercase(inputStrLower);

    if (strcmp(inputStrLower, "end") == 0)
    {
        printf("Cancelling...\n");
        return true; // Return true to indicate the "end" command was detected.
    }
    return false; // Return false if the input is not "end".
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
                return false;
            }
            i++;
        }
    }
    return true;
}

void convertToLowercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}

nnode *createNode(int num)
{
    nnode *newNode = (nnode *)malloc(sizeof(nnode));
    if (newNode)
    {
        newNode->number = num;
        newNode->next = NULL;
    }
    return newNode;
}

void insertNode(nnode **head, int num)
{
    nnode *newNode = createNode(num);
    if (!newNode)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        nnode *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printList(nnode *head)
{
    nnode *current = head;
    while (current != NULL)
    {
        printf("%d ", current->number);
        current = current->next;
    }
    printf("\n");
}

void freeList(nnode *head)
{
    nnode *current = head;
    while (current != NULL)
    {
        nnode *temp = current;
        current = current->next;
        free(temp);
    }
}