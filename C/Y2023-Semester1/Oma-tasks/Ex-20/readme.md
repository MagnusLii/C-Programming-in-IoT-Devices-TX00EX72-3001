Implement a program that consists of three files: main.c, debug.c and debug.h.
Debug.c contains two functions:

• set_debug_level
• dprintf

Set_debug_level takes one integer parameter (debug level) and stores the value in a static variable
that is accessible to all functions in the file but not globally.
```
void set_debug_level(int debug_level);
```
Dprintf works like printf but there is an extra integer parameter (debug level) that comes before the
usual printf parameters:
```
int dprintf(int debug_level, const char *fmt, …);
```
If dprintf debug level is lower or equal to the stored debug level then function prints the output to
stderr prefixed with [DBGx] , where x is the debug level given as parameter. Otherwise, function
prints nothing and returns zero.
Write a program that asks user to enter debug level in range 0 – 4 and calls set_debug_level with the
number. Then program prints five messages with a random debug level in range 0 – 4 using dprintf.
Get a new random debug level for each message. The messages must have at least one numerical
argument that is a running index that starts from one. First message has index 1,second message 2,
etc