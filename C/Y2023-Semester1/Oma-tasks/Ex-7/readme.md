Ex7
Write a function named read_positive. The function takes a pointer to int as a parameter and
returns a bool.

bool read_positive(int *value);

The function asks (=prints) user to enter a positive number. If user enters a valid number and the
number is positive the function stores the number using the pointer and returns true. If reading
number fails or number is not positive function returns false without making changes to the value
using pointer.
Write a program that “plays” a guessing game with user. The program asks user to guess how much
money it has and after the guess claims to twice as much plus 20 euros. Program must use the
function above to ask user to enter the number. If reading fails, function returns false, program asks
to enter a number again. Program stops when user has entered an incorrect value three times.

```
Guess how much money I have!
Enter a positive number: 50
You didn’t get it right. I have 120 euros.
Guess how much money I have!
Enter a positive number: xd
Incorrect input
Guess how much money I have!
Enter a positive number: 70
You didn’t get it right. I have 160 euros.
Guess how much money I have!
Enter a positive number: fubar
Incorrect input
Guess how much money I have!
Enter a positive number: not
Incorrect input
I give up! See you later!
```