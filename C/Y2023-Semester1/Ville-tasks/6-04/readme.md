Write a function called filter_alpha. The function takes three parameters:

    Character pointer (=string)
    String maximum size including terminating nul character
    Pointer to a function that takes no parameters and returns a character

Filter_alpha calls the function given as a parameter to read input one character at a time until a newline is read,  the maximum size is reached, or the read character is a zero. Function must ensure that the string is always properly terminated. Function saves the received character in the string only if the character is a letter. Filter alpha returns the total number of characters it has read. The count must include letters and other characters except zero.

My_getchar reads a character and returns it. When end of input occurs my_getchar returns zero.