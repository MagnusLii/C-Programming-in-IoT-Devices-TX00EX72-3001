


int main(void)
{
    int num1 = 0, num2 = 0;
    char operator[4] = {0};

    int *pnum1 = &num1, *pnum2 = &num2;
    char *poperator = operator;

    if (format_tester(line, sizeof(line), pnum1, pnum2, poperator) == 1)
    {
        /* code */
    }

    return 0;
}

int format_tester(const char *input, int size_of_string, int *pnum1, int *pnum2, char *poperator)
{
    if (sscanf(input, "%d %d %3s", *pnum1, *pnum2, *poperator) == 3)
    {
        if (strcmp(*poperator, "add") == 0 || strcmp(*poperator, "sub") == 0 ||
            strcmp(*poperator, "mul") == 0 || strcmp(*poperator, "div") == 0)
        {
            return 1; // Valid
        }
    }

    return 0; // Invalid
}

int error_finder(const char *input, int size_of_string, int *pnum1, int *pnum2, char *poperator)
{
    // the fuck do I do here now...................
    char string[256] = *input;
    int i = 0, num_of_spaces = 0;



    while (string[i] != '\0')
    {
        switch (num_of_spaces)
        {
        case 0:
            if (string[i] != )
            {
                /* code */
            }
            
            break;
        
        default:
            break;
        }

    }
}
