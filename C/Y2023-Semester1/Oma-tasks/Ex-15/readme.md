Ex15
Write a program that reads data from a text file into an array of structures. The size of the array is 40
elements.
The structure is defined as:

```
typedef struct menu_item_ {
char name[50];
double price;
} menu_item;
```

The program asks user to enter a filename. Then program tries to open the file in textmode for
reading. If opening the file fails the program prints an error message with the filename to stderr and
then exits.
Program reads data from the text file into the array until end of file or the array is full. Each row of
the file contains data of one structure. The members are separated by semicolons. For example:

```
Mega double burger with bacon and cheese; 23.50
Belgian style fries with vegan mayo; 5.60
```

When reading ends the program closes the file and prints the content of the array in formatted
columns. Price is printed into a field of eight characters with two decimals precision followed by the
name of the item. Unused elements are not printed.