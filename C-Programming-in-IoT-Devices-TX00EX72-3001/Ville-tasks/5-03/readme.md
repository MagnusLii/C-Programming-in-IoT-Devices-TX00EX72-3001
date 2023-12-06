Write a program that reads input lines using read_line-function. read_line returns true if input was read and false when input ends. Each line contains two numbers and a command "add", "sub", "mul", or "div". Program reads lines one by one and checks that input is valid (first 2 integers then a string that is a valid command). If line is not properly formatted the program prints the line and displays an error message. For each properly formatted line the program prints out a calculation which is determined by the command.

For example input that contains:
7 2 sub
43 -3 add

Will make the program print:
7 - 2 = 5
43 + -3 = 40

If the input can't be parsed then program must print the invalid line and an error message that describes the location of the error. See the example solution output for details.
