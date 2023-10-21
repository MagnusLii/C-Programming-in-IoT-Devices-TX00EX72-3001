Student Management System

This project is designed for the "C Programming in IoT Devices TX00EX72-3001" course. The student management system runs entirely in a command-line environment (CMD), and all data and records are stored in a plain text file.

Features
The system includes the following features:

    Ability to add or remove students from the record.
    Ability to modify existing records.
    Ability to search or browse existing records.

Please note that this program assumes that the database (DB) file has already been created and is named "db.txt." It's important to be aware that this project does not include extensive error handling or validation for scenarios where the DB file might become corrupted outside of the program.

Database (DB) Layout
The DB layout consists of the following elements:

row 1: (The current student index number) (The current DB entries tracking number)
row 2: Column names for the data
row 3: Individual student entries
...


Afterthoughts for future reference.

The code is dissapointing, without going into depth, the MAJOR goals I had starting this project were to make the code modular and to have nothing nested more than 3 times.

While the code started out very modular and it kept nesting below the 3 tier threshold as I went beyond and/or had to rework the plans I made before starting the project everything started to fall apart. As I needed to add more and more errorcheking to deal with the myriad ways C finds ways to cause errors or undefined behavior the nesting got worse and worse.
As I needed to refactor more and more code I failed to foresee and find the balance between modularity and simplicity.