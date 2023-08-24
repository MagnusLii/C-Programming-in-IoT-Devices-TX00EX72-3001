Write a function that takes three integer pointers as parameters. The function sorts the three numbers in ascending order.

The prototype of the function is:

void sort3(int *a, int *b, int *c);

For example if we have following:

int n1 = 5;

int n2 = 3;

int n3 = 9;

sort3(&n1, &n2, &n3);

printf("%d, %d, %d", n1, n2, n3);

The program prints: 3, 5, 9
