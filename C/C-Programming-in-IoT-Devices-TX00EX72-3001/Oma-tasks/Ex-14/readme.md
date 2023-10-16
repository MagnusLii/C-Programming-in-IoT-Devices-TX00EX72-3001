Write a program that reads lines of text in to an array of strings. The maximum length of the line is
80 characters and the maximum number of lines is 100.
The program asks user to enter a filename. Then program tries to open the file in textmode for
reading. If opening the file fails the program prints an error message with the filename to stderr and
then exits. The program reads lines from the file into the array until the file ends or the array is full
(100 lines read). Then program closes the file.
Then program converts all letters to upper case (see toupper() – function).
Then program opens the file for writing in text mode and writes all read lines to file and closes the
file.