Ex5
Write a function read_range that takes two integer parameters and returns an integer. The function
asks user to enter a number in the range given as parameters. The first parameter is the lowest
acceptable value and the second is the highest acceptable value. When user enters a number the
input is validated in the following ways:
    • A number was successfully read
    • The number is in the specified range
If the input is invalid then a descriptive error message is printed and user is asked to enter number
again. The function returns only if correct input is given. The function returns the entered number.
The declaration of the function is:

```
int read_range(int low, int high);
```

Write a program that “plays” a dice game with user. The game consists of three rounds where
program asks user to roll a die and enter the result (1 – 6). The program must use read_range to
validate input.
The program cheats and always claims to have one higher than what user rolled except when user
rolls six – then program says that it is a tie.

```
Let’s play!
Roll a die and enter your result.
Enter a number between 1 and 6: 2
I got 3. I win!
Roll a die and enter your result.
Enter a number between 1 and 6: 5
I got 6. I win!
Roll a die and enter your result.
Enter a number between 1 and 6: 6
I got 6. It is a tie!
Better luck next time. Bye!
```