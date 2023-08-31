void sort3(int *a, int *b, int *c);

int main(void)
{
    int n1 = 5;
    int n2 = 3;
    int n3 = 9;
    
    sort3(&n1, &n2, &n3);
    
    printf("%d, %d, %d", n1, n2, n3);
    
    return 0;
}

void sort3(int *a, int *b, int *c)
{
    int temp;
    if (*a > *b)
    {
        temp = *a;
        *a = *b;
        *b = temp;
    }
    if (*b > *c)
    {
        temp = *b;
        *b = *c;
        *c = temp;
    }
    if (*a > *b)
    {
        temp = *a;
        *a = *b;
        *b = temp;
    }
}

