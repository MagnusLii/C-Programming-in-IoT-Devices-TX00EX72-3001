/*Write a function that takes three integer pointers as parameters. The function sorts the three numbers in ascending order.

The prototype of the function is:

void sort3(int *a, int *b, int *c);

For example if we have following:

int n1 = 5;

int n2 = 3;

int n3 = 9;

sort3(&n1, &n2, &n3);

printf("%d, %d, %d", n1, n2, n3);

The program prints: 3, 5, 9
*/

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

