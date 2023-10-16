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