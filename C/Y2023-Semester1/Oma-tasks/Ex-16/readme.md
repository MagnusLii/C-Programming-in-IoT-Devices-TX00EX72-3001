Write a program that asks user to enter numbers or to enter end to stop entering numbers. If user
enters any other text except end the program prints an error message and discards the input. The
program must store the numbers in a linked list where each node is dynamically allocated.

```
typedef struct node {
int number;
struct node *next;
} nnode;
```

When user enters end the program prints all entered numbers, frees the allocated memory and then
ends.