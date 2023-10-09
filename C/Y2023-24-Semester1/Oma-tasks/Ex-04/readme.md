Ex4
Write a function called read_integer that takes no parameters and returns an integer. The function
reads a single integer. If the reading fails the function prints “invalid input” and tries to read an
integer again. When an integer is successfully read the function returns the integer.
The declaration of the function is:
```
int read_integer(void);
```
Write a program that asks user to enter positive integers or a negative number to stop. The program
must use read_integer-function to read the number. The program calculates and prints the average
of the entered numbers excluding the negative number. For example, if user enters: 1 3 4 2
7 -1, the program prints 3.4 as the average.
```
The program must print the average with three decimal precision.
Enter positive numbers or negative to stop: 1
Enter positive numbers or negative to stop: 3
Enter positive numbers or negative to stop: 4
Enter positive numbers or negative to stop: 2
Enter positive numbers or negative to stop: 7
Enter positive numbers or negative to stop: -1
You entered 5 positive numbers. The average is: 3.400```