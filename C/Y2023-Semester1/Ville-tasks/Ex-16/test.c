int main(void)
{

    char input[255];
    size_t size = sizeof(input);
    char *pinput = input;
    int *psize = &size;
    bool x = false;

    x = read_line(pinput, size);
    
    printf("%s", input);

    return 0;
}