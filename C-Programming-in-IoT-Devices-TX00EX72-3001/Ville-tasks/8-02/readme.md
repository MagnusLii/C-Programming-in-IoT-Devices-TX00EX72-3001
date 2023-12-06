Implement a function that takes two parameters: a number to print and field width. The function prints the number in binary with the specified field width filling the field with leading zeros if necessary. If printing requires more space than the field width specifies the number is still printed in full. The function must not print any white space before or after the number.

For example:
```
print_binaryw(5, 4) prints 0101

print_binaryw(0x27, 4) prints 100111

print_binaryw(5, 0) prints 101
```