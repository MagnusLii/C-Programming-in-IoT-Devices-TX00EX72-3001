#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>

// #defines
#define DB "db.txt"
#define TEMP "temp.tmp"
#define NAME_LENGHT 21
#define STUDENT_ID_LENGHT 13
#define DEFAULT_STRING_LENGHT 21
#define INPUT_BUFFER_LENGHT 1024 // Used for any array that is expected to store userinput.
#define LONG_STRING_LENGHT 1024  // Used for any array that is expected to store long strings but not nessecarily userinput.
#define SEPARATOR "\n\n----------------------------------------\n\n"
#define MAJORS                                                       \
    {                                                                \
        "Biomimicry", "PuppetArts", "BicycleDesign", "EcoGastronomy" \
    }
#define NUM_MAJORS 4
#define DB_COLUMN_COUNT 5

struct Student
{
    int studentind, db_entry_row;
    char firstname[LONG_STRING_LENGHT];
    char lastname[LONG_STRING_LENGHT];
    char studentid[LONG_STRING_LENGHT];
    char major[LONG_STRING_LENGHT];
    int fetchFailure;
};

typedef struct Student Student;

// prototypes
bool getDBRowInd(int *pStudentind, int *pDB_rows);
bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString);
bool stringToIntConv(const char *str, int *result);
bool stringToDoubleConv(const char *inputStr, double *result);
void fgetsStringWhileLoopAlphanumerical(const char *stringToPrint, const char *retryMessage, char *stringToStoreTo, const int maxLenghtOfString);
bool dtimeString(char *stringToStoreTo, const size_t bufferSize);
bool createStudentId(struct Student *student);
void addNewStudent();
int getIndNum(const char *buffer);
void editStudentEntry();
bool addNewEntryToDB(struct Student studentStruct);
FILE *openFileWithRetry(const char *fileName, const char *mode, int maxRetries);
void deleteStudentEntry();
void lookupOrBrowse();
bool stringIsYesOrNo(const char *str);
void browseStudentList();
void lookupStudent();
void convertToLowercase(char *str);
Student fetchStudentData(const int studentind);
bool verifyTokenLen(const char *token, const int maxLen);
bool modifyEntryToDB(struct Student studentStruct);
bool exitToCancel(const char *inputStr, const int strMaxLen);
bool chooseMajor(char *stringToStoreTo);
void updateDatabase(const char *currentDBFileName, const char *tempDBFileName);

int main(){
    int switch_choice = 0;
    char choice_str[INPUT_BUFFER_LENGHT] = "\0";
    bool exit = false; // Flag to control program exit.
    while (exit == false)
    {
        switch_choice = 0;
        printf("%s"
                "Student record management system\n\n"
               "Menu:\n"
               "1. Add new student\n"
               "2. Edit student\n"
               "3. Delete student\n"
               "4. Search student\n"
                "5. Exit\n"
               "Enter your choice: ",
               SEPARATOR);
        improvedFgets(choice_str, DEFAULT_STRING_LENGHT);
        stringToIntConv(choice_str, &switch_choice);


        switch (switch_choice)
        {
        case 1:
            addNewStudent();
            break;
        case 2:
            editStudentEntry();
            break;
        case 3:
            deleteStudentEntry();
            break;
        case 4:
            lookupOrBrowse();
            break;
        case 5:
            printf("Terminating program...\n");
            exit = true;
            break;
        default:
            fprintf(stderr, "Error: Incorrect input enter number from 1-5.\n");
            break;
        }
        switch_choice = 0;
    }
    return 0;
}

/**
 * Fetches student data from the database and returns it as a Struct.
 * Requires studentind to find the correct data.
 *
 * @param studentind - The index of the student record to fetch. The index num is the leftmost column in the database.
 *
 * @return A Student struct containing the fetched data.
 */
Student fetchStudentData(const int studentind){
    Student student = {0};
    int tokencount = 0, linecount = 0;
    char *token, buffer[LONG_STRING_LENGHT] = "\0";
    FILE *dbFile = openFileWithRetry(DB, "r", 3);

    student.fetchFailure = 0; // Set fetchFailure flag to 0 to indicate no failure.

    if (studentind < 0){
        fprintf(stderr, "Error: Invalid student index number.\n");
        printf("Cancelling...");
        student.fetchFailure = 1; // Set flag to indicate a failure.
        return student;
    }
    
    if (dbFile == NULL){
        fprintf(stderr, "Error: unable to open file %s", DB);
        printf("Cancelling...");
        student.fetchFailure = 1; // Set flag to indicate a failure.
        return student;
    }

    // Skip the first 3 lines of the database file as they contain metadata.
    fgets(buffer, LONG_STRING_LENGHT, dbFile);
    fgets(buffer, LONG_STRING_LENGHT, dbFile);

    bool entry_found = false; // Flag to indicate if the entry is found in the database.
    while ((fgets(buffer, LONG_STRING_LENGHT, dbFile)) != NULL && entry_found == false){
        linecount++;
        if (getIndNum(buffer) == studentind){
            token = strtok(buffer, ",");
            while (token != NULL){
                switch (tokencount){
                case 0:
                    stringToIntConv(token, &student.studentind);
                    break;
                case 1:
                    strncpy(student.firstname, token, NAME_LENGHT - 1);
                    student.firstname[NAME_LENGHT - 1] = '\0';
                    break;
                case 2:
                    strncpy(student.lastname, token, NAME_LENGHT - 1);
                    student.lastname[NAME_LENGHT - 1] = '\0';
                    break;
                case 3:
                    strncpy(student.studentid, token, STUDENT_ID_LENGHT - 1);
                    student.studentid[STUDENT_ID_LENGHT - 1] = '\0';
                    break;
                case 4:
                    strncpy(student.major, token, LONG_STRING_LENGHT - 1);
                    student.major[LONG_STRING_LENGHT - 1] = '\0';
                    break;
                }

                token = strtok(NULL, ", ");
                tokencount++;
            }
            student.db_entry_row = linecount + 2; // Set the row number in the database where the entry was found. (+2) to account for the 2 lines skipped at the beginning.
            entry_found = true;
        }
    }
    fclose(dbFile);

    if (entry_found == false)
    {
        fprintf(stderr, "Error: Student record with index %d not found.\n", studentind);
        printf("Cancelling...");
        student.fetchFailure = 1; // Set fetchFailure flag to indicate a failure.
        return student;
    }
    return student;
}

/**
 * Verifies that the length of a token does not exceed a specified maximum length.
 *
 * @param token   - The token to be verified.
 * @param maxLen  - The maximum allowed length for the token.
 *
 * @return        - Returns true if the token length is valid (within maxLen), false otherwise.
 */
bool verifyTokenLen(const char *token, const int maxLen){
    if (token == NULL || maxLen < 1){
        fprintf(stderr, "Error: Invalid pointer or maxLen in verifyTokenLen.\n");
        return false;
    }

    if (strlen(token) > maxLen){
        fprintf(stderr, "Error: Token length exceeds max length of %d characters.\n", maxLen);
        return false;
    }
    return true;
}

/**
 * Gets the current index for students and the number of entries (rows) in the database.
 * Returns false if unable to open the database file or read the correct data.
 *
 * @param pStudentind - Pointer to an integer to store the current student index.
 * @param pDB_rows    - Pointer to an integer to store the number of entries in the database.
 *
 * @return true if data is successfully read and stored, false otherwise.
 */
bool getDBRowInd(int *pStudentind, int *pDB_rows){
    if (pStudentind == NULL || pDB_rows == NULL){
        fprintf(stderr, "Error: Invalid pointer in getDBRowInd.\n");
        return false;
    }

    FILE *pFile = openFileWithRetry(DB, "r", 3);
    if (pFile == NULL){
        return false;
    }
    
    int readCount = fscanf(pFile, "%d %d", pStudentind, pDB_rows);
    if (readCount != 2){ // Check if the correct number of values was successfully read from the file.
        printf("Error: Failed to read data from the file '%s'\n", DB);
        return false;
    }
    fclose(pFile);
    return true;
}

/**
 * A modified version of the fgets() function that replaces '\n' with '\0'.
 * It does not accept empty strings and checks if user input exceeds the maximum length.
 *
 * @param stringToStoreTo - A pointer to the character buffer where the input will be stored.
 * @param maxLenghtOfString - The maximum length of the input string.
 *
 * @return true if input is successfully read and meets the conditions, false otherwise.
 */
bool improvedFgets(char *stringToStoreTo, const int maxLenghtOfString){
    if (stringToStoreTo == NULL || maxLenghtOfString < 1){
        fprintf(stderr, "Error: Invalid pointer or maxLenghtOfString in improvedFgets.\n");
        return false;
    }
    
    bool newline_found = false; // Flag to track the presence of '\n'.
    char userinput[INPUT_BUFFER_LENGHT] = "\0";

    if (fgets(userinput, LONG_STRING_LENGHT, stdin) != NULL) {
        // Check if the input contains a newline character.
        char *newline_position = strchr(userinput, '\n');
        if (newline_position != NULL) {
            *newline_position = '\0';
        }
        // Verify input len.
        if (strlen(userinput) >= maxLenghtOfString) {
            fprintf(stderr, "Error: Input over max accepted length of %d characters.\n", maxLenghtOfString - 1);
            return false;
        }
        strncpy(stringToStoreTo, userinput, maxLenghtOfString - 1);
        stringToStoreTo[maxLenghtOfString - 1] = '\0'; // Ensure null-termination.
        return true;
    }
    else {
        fprintf(stderr, "Error: Failed to read input.\n");
        return false;
    }
}

/**
 * Converts a string to an integer and returns true if the conversion is successful,
 * does not allow partial conversions, and validates the input.
 * Doesn't allow for negative numbers as they won't be needed in this program.
 *
 * @param str - The input string to be converted.
 * @param result - Pointer to an integer where the result will be stored.
 *
 * @return true if the conversion is successful and the input is valid, false otherwise.
 */
bool stringToIntConv(const char *str, int *result){
    if (str == NULL || result == NULL){
        fprintf(stderr, "Error: Invalid pointer in stringToIntConv.\n");
        return false;
    }

    char *endptr; // Pointer to the character after the converted part of the string.
    errno = 0;    // Setting errno to 0 to detect errors in strtol().
    long int num = strtol(str, &endptr, 10);  // #TODO: replace the 10 with something better

    // Checking if input is beyond the range of a long integer.
    if (errno == ERANGE){
        fprintf(stderr, "Error: could not complete conversion to integer, number out of range.\nEnter a number between %d and %d.\n", LONG_MIN, LONG_MAX);
        return false;
    }
    // Checking if input contained anything other than numbers.
    else if (*endptr != '\0'){
        // Checking if input is non numerical.
        for (char *p = endptr; *p != '\0'; p++){
            if (isdigit((unsigned char)*p) == false){
                return false;
            }
        }
        fprintf(stderr, "Error: could not read an integer.\n");
        return false;
    }
    *result = num;
    return true;
}

/**
 * Converts a string to a double and returns true if the conversion is successful,
 * checks that input is within the range, and validates the conversion.
 *
 * @param inputStr - The input string to be converted.
 * @param result - Pointer to a double where the result will be stored.
 *
 * @return true if the conversion is successful and the input is valid, false otherwise.
 */
bool stringToDoubleConv(const char *inputStr, double *result){
    if (inputStr == NULL || result == NULL){
        fprintf(stderr, "Error: Invalid pointer in stringToDoubleConv.\n");
        return false;
    }

    char *endptr; // Pointer to the character after the converted part of the string.
    errno = 0;
    double num = strtod(inputStr, &endptr);

    // Checking if input is beyond the range of a double.
    if (errno == ERANGE){
        fprintf(stderr, "Error: could not complete conversion to double, number out of range.\n"
               "Enter a number between %f and %f.\n",
               DBL_MIN, DBL_MAX);
        return false;
    }
    // Checking if input contained anything other than numbers.
    else if (*endptr != '\0'){
        fprintf(stderr, "Error: could not complete conversion to double\n");
        return false;
    }
    *result = num;
    return true;
}

/**
 * Template for a while loop using fgets that only allows alphanumeric characters.
 * Displays error messages and error locations in case of input errors.
 *
 * @param stringToPrint - The message to print to the user before input.
 * @param retryMessage - The message to print when input is invalid, prompting the user to retry.
 * @param stringToStoreTo - A pointer to the character buffer where the input will be stored.
 * @param maxLenghtOfString - The maximum length of the input string.
 */
void fgetsStringWhileLoopAlphanumerical(const char *stringToPrint, const char *retryMessage, char *stringToStoreTo, const int maxLengthOfString){
    if (stringToPrint == NULL || retryMessage == NULL || stringToStoreTo == NULL || maxLengthOfString  < 1){
        fprintf(stderr, "Error: Invalid pointer in fgetsStringWhileLoopAlphanumerical.\n");
        return;
    }
    if (maxLengthOfString < 1){
        fprintf(stderr, "Error: Invalid maxLenghtOfString in fgetsStringWhileLoopAlphanumerical.\n");
        return;
    }

    bool input_valid = false;
    while (input_valid == false){
        printf("%s", stringToPrint);
        printf("Or input 'Exit' to cancel.\n"
               "Your input: ");
        input_valid = improvedFgets(stringToStoreTo, maxLengthOfString);

        // Alphanumerical character validation.
        // TODO: Has 6 levels on indentation, should be refactored.
        int charIndex = 0;
        while (input_valid == true && stringToStoreTo[charIndex] != '\0'){
            if (isalnum(stringToStoreTo[charIndex]) == false){
                // Display error message indicating the non-alphanumerical character and its location.
                printf("\nNon alphanumerical character detected at character %d (\"%c\")\n"
                       "Your input: %s\n"
                       "Error loc:  ",
                       charIndex + 1, stringToStoreTo[charIndex], stringToStoreTo);

                // Print '^' under the error location.
                for (int errorPosition = 0; errorPosition < charIndex; errorPosition++){
                    printf(" ");
                }
                printf("^\n");
                printf(SEPARATOR);
                input_valid = false;
            }
            charIndex++;
        }
        if (input_valid == false){
            printf("%s", retryMessage); // Prompting user to try again.
        }
    }
}

/**
 * Formats the current date and time as "YYYYMM" and stores it in the 'stringToStoreTo' buffer.
 *
 * This function retrieves the current date and time, formats it as "YYYYMM" (e.g., "202210"),
 * and stores the result in the provided buffer. The buffer size is determined by 'bufferSize'.
 *
 * @param stringToStoreTo - The buffer where the formatted date and time will be stored.
 * @param stringToStoreToSize - Size of the buffer where the formatted date and time will be stored.
 *
 * @return true if the formatting and storage were successful, false otherwise.
 *         Returns false if 'stringToStoreTo' is a null pointer or if 'bufferSize' is less than 7.
 */
bool dtimeString(char *stringToStoreTo, const size_t stringToStoreToSize){
    if (stringToStoreTo == NULL) {
        fprintf(stderr, "Error: Invalid pointer in dtimeString.\n");
        return false;
    }

    // Ensure that the provided buffer is large enough to store the formatted date and time.
    if (stringToStoreToSize < 7) {
        fprintf(stderr, "Error: Buffer string too short in dtimeString.\n");
        return false;
    }

    // Create and store the formatted string.
    time_t current_time = time(NULL);
    strftime(stringToStoreTo, stringToStoreToSize, "%Y%m", localtime(&current_time));
    return true;
}



/**
 * Checks if the input string is equal to "exit" (case-insensitive) and returns true if so.
 *
 * This function performs a case-insensitive comparison of the input string with the "exit" command.
 * It does not modify the input string.
 *
 * @param inputStr - The input string to check for the "exit" command.
 * @param strMaxLen - The maximum length to consider in the input string.
 *
 * @return true if the input is equal to "exit" (case-insensitive), false otherwise.
 *         Returns false if the input string is not exactly four characters long.
 *         Returns false if the input string is a null pointer.
 */
bool exitToCancel(const char *inputStr, const int strMaxLen) {
    // str cannot be "exit" if it is not exactly four characters long.
    if (strMaxLen != 4) {
        return false;
    }

    if (inputStr == NULL) {
        fprintf(stderr, "Error: Invalid pointer in exitToCancel.\n");
        return false;
    }

    // Perform a case-insensitive comparison with the "exit" command.
    if (strncasecmp("exit", inputStr, strMaxLen) == 0) {
        printf("Cancelling...\n");
        return true;
    }

    return false;
}


/**
 * Generates a unique student ID, updates student index, and stores relevant data in the Student struct.
 *
 * @param student - Pointer to a Student struct where the generated ID and data will be stored.
 *
 * @return true if the ID is successfully generated and data is stored, false otherwise.
 */
bool createStudentId(struct Student *student){
    if (student == NULL){
        fprintf(stderr, "Error: Invalid pointer in createStudentId.\n");
        return false;
    }

    char date_string[INPUT_BUFFER_LENGHT] = "\0";
    // Fetch the current date and format it as a string.
    dtimeString(date_string, sizeof(date_string));
    student->studentid[0] = '\0'; // Ensuring the student ID is empty before concatenating.
    strcat(student->studentid, date_string);

    // Fetch and update the student index and database row information.
    if (getDBRowInd(&student->studentind, &student->db_entry_row) == false){
        return false;
    }
    student->studentind++;
    student->db_entry_row++;

    // Convert the modified student index to a string with leading zeros to ensure ID len matches expected format.
    char modifiedStudentIndStr[INPUT_BUFFER_LENGHT] = "\0";
    sprintf(modifiedStudentIndStr, "%06d", student->studentind);

    // Concatenate the modified student index to the student ID.
    strcat(student->studentid, modifiedStudentIndStr);
    return true;
}

/**
 * Prompts the user to choose a major and stores the selected major in 'stringToStoreTo'.
 * Returns false if the user cancels midway.
 * Performs error checking to ensure the input is a valid integer within the specified range.
 *
 * @param stringToStoreTo - A pointer to the character buffer where the selected major will be stored.
 *
 * @return true if a major is successfully selected and stored, false if the user cancels or provides invalid input.
 */
bool chooseMajor(char *stringToStoreTo){
    if (stringToStoreTo == NULL){
        fprintf(stderr, "Error: Invalid pointer in chooseMajor.\n");
        return false;
    }

    char userinput[INPUT_BUFFER_LENGHT] = "\0";
    int userinput_int;
    // Array of major options defined in MAJORS #define.
    char majors[NUM_MAJORS][LONG_STRING_LENGHT] = MAJORS;
    char loopMsg[LONG_STRING_LENGHT] = "Choose major\n";
    char errorMsg[LONG_STRING_LENGHT] = "Please enter a valid choice. [Single integer number]\n";

    // Build the message for user selection by iterating through major options.
    for (int i = 0; i < NUM_MAJORS; i++){
        snprintf(loopMsg, sizeof(loopMsg), "%s%d", loopMsg, i + 1);
        strcat(loopMsg, ". ");
        strcat(loopMsg, majors[i]);
        strcat(loopMsg, "\n");
    }

    // Prompt the user to choose a major.
    bool input_valid = false; // Flag to track the validity of the input.
    while (input_valid == false){
        fgetsStringWhileLoopAlphanumerical(loopMsg, errorMsg, userinput, DEFAULT_STRING_LENGHT);
        if (exitToCancel(userinput, strlen(userinput)) == true){
            return false;
        }

        input_valid = stringToIntConv(userinput, &userinput_int);
        if (input_valid == false){
            printf("%s", errorMsg);
        }
        else if (userinput_int < 1 || userinput_int > NUM_MAJORS){
            fprintf(stderr, "\nError: Enter a number between 1 an %d\n", NUM_MAJORS);
            input_valid = false; // Set input_valid to false for out-of-range input.
        }
    }

    // Copy the selected major to the output string.
    strncpy(stringToStoreTo, majors[userinput_int - 1], LONG_STRING_LENGHT - 1);
    stringToStoreTo[LONG_STRING_LENGHT - 1] = '\0';
    return true; // Return true to indicate successful selection and storage of a major.
}

/**
 * Converts all characters in a string to lowercase.
 *
 * @param str - A pointer to the null-terminated string to be converted to lowercase.
 */
void convertToLowercase(char *str){
    if (str == NULL){
        fprintf(stderr, "Error: Invalid pointer in convertToLowercase.\n");
        return;
    }

    for (int i = 0; str[i] != '\0'; i++){
        str[i] = tolower(str[i]);
    }
}

/**
 * Adds a new student entry to the database file and updates the database.
 *
 * @param studentStruct - The Student struct representing the new entry to be added.
 *
 * @return true if the new entry is successfully added and the database is updated, false otherwise.
 */
bool addNewEntryToDB(struct Student studentStruct){
    // Open a temporary file for writing.
    FILE *tmpFile = openFileWithRetry(TEMP, "w", 3);
    if (tmpFile == NULL){
        fprintf(stderr, "Error: unable to open file %s", TEMP);
        return false; // Return false if unable to open the temporary file.
    }

    // Open the database file for reading.
    FILE *pFile = openFileWithRetry(DB, "r", 3);
    if (pFile == NULL){
        fprintf(stderr, "Error: unable to open file %s", DB);
        return false; // Return false if unable to open the database file.
    }

    int rowCount = 0;                        // Variable to count the number of rows in the database.
    char buffer[INPUT_BUFFER_LENGHT] = "\0"; // Buffer to store lines read from the database.

    // Copy existing data to the temporary file while adding the new entry.
    while (fgets(buffer, INPUT_BUFFER_LENGHT, pFile) != NULL){
        rowCount++;

        // Write the student index and database row information in the first row of the temp file.
        if (rowCount == 1){
            fprintf(tmpFile, "%d %d\n", studentStruct.studentind, studentStruct.db_entry_row);
        }
        else{
            fprintf(tmpFile, "%s", buffer); // Copy the existing data to the temp file.
        }
    }

    // Add the new student entry to the temp file.
    fprintf(tmpFile, "\n%d,%s,%s,%s,%s", studentStruct.studentind, studentStruct.firstname, studentStruct.lastname, studentStruct.studentid, studentStruct.major);

    fclose(pFile);   // Close the database file.
    fclose(tmpFile); // Close the temporary file.

    updateDatabase(DB, TEMP);

    return true; // Return true to indicate a successful entry addition and database update.
}

/**
 * Modifies an existing entry in the database file using data from the provided Student struct.
 * The entry to be modified is located based on the 'db_entry_row' field stored in the struct.
 *
 * @param studentStruct - The Student struct containing the modified entry data.
 *
 * @return true if the specified entry is found and successfully modified, false otherwise.
 */
bool modifyEntryToDB(struct Student studentStruct){
    printf("Modifying data.\n");

    // Open a temporary file for writing.
    FILE *tmpFile = openFileWithRetry(TEMP, "w", 3);
    if (tmpFile == NULL){
        return false; // Return false if unable to open the temporary file.
    }

    // Open the database file for reading.
    FILE *pFile = openFileWithRetry(DB, "r", 3);
    if (pFile == NULL){
        return false; // Return false if unable to open the database file.
    }

    int rowCount = 0;                        // Variable to count the number of rows in the database.
    char buffer[INPUT_BUFFER_LENGHT] = "\0"; // Buffer to store lines read from the database.
    bool entry_found = false;                // Flag to indicate if the specified entry is found.

    // Copy existing data to the temporary file while replacing the specified entry with new data.
    while ((fgets(buffer, INPUT_BUFFER_LENGHT, pFile)) != NULL){
        rowCount++;

        // Replace the specified entry with the new entry data.
        if (rowCount == studentStruct.db_entry_row){
            fprintf(tmpFile, "%d,%s,%s,%s,%s", studentStruct.studentind, studentStruct.firstname, studentStruct.lastname, studentStruct.studentid, studentStruct.major);
            entry_found = true;
        }

        // Copy the existing data to the temporary file for other entries.
        else{
            fprintf(tmpFile, "%s", buffer);
        }
    }

    fclose(pFile);
    fclose(tmpFile);

    updateDatabase(DB, TEMP);

    return entry_found; // Return true if the specified entry is found and modified, false otherwise.
}

/**
 * Function to add a new student entry to the database.
 * Prompts the user to enter student information, including first name, last name, and major.
 * Generates a unique student ID and adds the new student to the database.
 */
void addNewStudent(){
    // Gathering new student information.
    struct Student newStudent;
    char inputstr[LONG_STRING_LENGHT] = "\0";

    // Creating firstname.
    sprintf(inputstr, "Enter firstname (max %d alphanumerical characters only!)\n", NAME_LENGHT - 1);
    char errorMsg[LONG_STRING_LENGHT] = "Please enter a valid firstname.\n";
    printf("%s", SEPARATOR);
    fgetsStringWhileLoopAlphanumerical(inputstr, errorMsg, newStudent.firstname, NAME_LENGHT);
    if (exitToCancel(newStudent.firstname, strlen(newStudent.firstname)) == true){
        return;
    }

    // Creating lastname.
    sprintf(inputstr, "Enter lastname (max %d alphanumerical characters only!)\n", NAME_LENGHT - 1);
    sprintf(errorMsg, "Please enter a valid lastname.\n");
    printf("%s", SEPARATOR);
    fgetsStringWhileLoopAlphanumerical(inputstr, errorMsg, newStudent.lastname, NAME_LENGHT);
    if (exitToCancel(newStudent.lastname, strlen(newStudent.firstname)) == true){
        return;
    }

    createStudentId(&newStudent);

    // Choosing major.
    printf("%s", SEPARATOR);
    if (chooseMajor(newStudent.major) == false){
        return;
    }

    printf("%s", SEPARATOR);
    char userinput[INPUT_BUFFER_LENGHT] = "\0";
    // Confirming student data.
    do{
        printf("Student will be added with the following information\n"
               "Firstname: %s\n"
               "Lastname: %s\n"
               "Studentid: %s\n"
               "Major: %s\n",
               newStudent.firstname, newStudent.lastname, newStudent.studentid, newStudent.major);
        printf("Is this correct? [yes/no]\n");
        printf("Input: ");
        improvedFgets(userinput, DEFAULT_STRING_LENGHT);
        convertToLowercase(userinput);
    } while (stringIsYesOrNo(userinput) == false);

    if (stricmp(userinput, "no") == 0 || stricmp(userinput, "n") == 0){
        printf("Cancelling...\n");
        return;
    }

    printf("Adding new student to DB...\n");
    if (addNewEntryToDB(newStudent) == false){
        fprintf(stderr, "Error: Failed to add new student to DB.\n");
        printf("Exiting...\n");
        return;
    }
    printf("New student added to DB.\n");
    return;
}

/**
 * Function to extract the index number from a database entry string.
 *
 * @param buffer - The input string containing the database entry.
 *
 * @return The extracted index number as an integer or "-1" if an error occurs.
 */
int getIndNum(const char *buffer){ 
    if (buffer == NULL){
        fprintf(stderr, "Error: Invalid pointer in getIndNum.\n");
        return -1;
    }

    if (strlen(buffer) > INPUT_BUFFER_LENGHT){
        fprintf(stderr, "Error: Buffer string too long in getIndNum.\n");
        return -1;
    }    

    char index_number[INPUT_BUFFER_LENGHT];
    int numberlength = 0;
    int index = 0;
    while (index < strlen(buffer) && buffer[index] != ','){
        index_number[numberlength] = buffer[index];
        numberlength++;
        index++;
    }
    index_number[numberlength] = '\0';
    
    int result = 0;
    if (stringToIntConv(index_number, &result) == false){
        fprintf(stderr, "Error: Failed to convert buffer to int in getIndNum.\n");
        return -1;
    }

    /*  Converting string to int.
        This has the potential to fail if the characters before the first "," are not numbers.
        However error checks in other functions should prevent any incorrectly formatted DB entries.*/
    return result;
}

/**
 * Function to edit an existing student entry in the database.
 * Allows the user to select a student by index number and modify the first name, last name, or major.
 */
void editStudentEntry(){
    char userinput[INPUT_BUFFER_LENGHT] = "\0";
    int studentind = 0;
    char promptMsg[LONG_STRING_LENGHT] = "\nEnter student index number (Leftmost column in DB).\n";
    char errorMsg[LONG_STRING_LENGHT] = "Please enter a valid student index number.\n";

    // Prompting user to enter student index number.
    bool input_valid = false;
    while (input_valid == false){
        printf(SEPARATOR);
        fgetsStringWhileLoopAlphanumerical(promptMsg, errorMsg, userinput, DEFAULT_STRING_LENGHT);
        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }
        input_valid = stringToIntConv(userinput, &studentind);
        if (input_valid == false){
            fprintf(stderr, "Error: Enter a valid integer.\n");
        }
    }

    // Fetching current student data into struct.
    struct Student student = fetchStudentData(studentind);
    if (student.fetchFailure == 1){
        printf("Cancelling...\n");
        return;
    }

    // Confirming what information to edit with user.
    printf("%s", SEPARATOR);
    sprintf(promptMsg, "\n1. Firstname\n2. Lastname\n3. Major\nChoose data to change.\n");
    sprintf(errorMsg, "Enter a valid integer between range 1-3.\n");
    int choice = 0;
    input_valid = false;
    while (input_valid == false){
        fgetsStringWhileLoopAlphanumerical(promptMsg, errorMsg, userinput, DEFAULT_STRING_LENGHT);
        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }
        input_valid = stringToIntConv(userinput, &choice);
        if (choice < 1 || choice > 3){
            fprintf(stderr, "Error: Enter a valid integer between range 1-3.\n");
            printf(SEPARATOR);
            input_valid = false;
        }
    }

    printf("%s", SEPARATOR);
    char inputstr[INPUT_BUFFER_LENGHT] = "\0";
    input_valid = false;
    switch (choice){
        // Changing firstname.
    case 1:
        sprintf(inputstr, "Enter firstname (max %d alphanumerical characters only!)\n", NAME_LENGHT - 1);
        fgetsStringWhileLoopAlphanumerical(inputstr, "Please enter a valid firstname.\n", userinput, NAME_LENGHT);
        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }
        for (int i = 0; i < strlen(userinput); i++){
            student.firstname[i] = userinput[i];
        }
        break;

        // Changing lastname.
    case 2:
        sprintf(inputstr, "Enter lastname (max %d alphanumerical characters only!)\n", NAME_LENGHT - 1);
        fgetsStringWhileLoopAlphanumerical(inputstr, "Please enter a valid lastname.\n", userinput, NAME_LENGHT);
        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }
        for (int i = 0; i < strlen(userinput); i++){
            student.lastname[i] = userinput[i];
        }
        break;

        // Swapping major.
    case 3:
        char majors[NUM_MAJORS][LONG_STRING_LENGHT] = MAJORS;
        printf("Current major: %s\n", student.major);
        printf("Choose new major\n");
        for (int i = 0; i < NUM_MAJORS; i++){
            printf("%d. %s\n", i + 1, majors[i]);
        }

        sprintf(inputstr, "Enter a number between 1 and %d.\n", NUM_MAJORS);
        sprintf(errorMsg, "Please enter a valid integer between range 1-%d.\n", NUM_MAJORS);
        fgetsStringWhileLoopAlphanumerical(inputstr, errorMsg, userinput, DEFAULT_STRING_LENGHT);
        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }

        int userinput_int = 0;
        while (input_valid == false){
            stringToIntConv(userinput, &userinput_int);
            if (userinput_int >= 1 && userinput_int <= NUM_MAJORS){
                input_valid = true;
            }
        }

        strncpy(student.major, majors[userinput_int - 1], LONG_STRING_LENGHT - 1);
        student.major[LONG_STRING_LENGHT - 1] = '\0';
        break;
    }

    // Confirming student record.
    printf("%s", SEPARATOR);
    do{
        printf("Student will be modified with the following information\n"
               "Firstname: %s\n"
               "Lastname: %s\n"
               "Studentid: %s\n"
               "Major: %s\n",
               student.firstname, student.lastname, student.studentid, student.major);
        printf("Is this correct? [yes/no]\n");
        printf("Input: ");
        improvedFgets(userinput, DEFAULT_STRING_LENGHT);
        convertToLowercase(userinput);
    } while (stringIsYesOrNo(userinput) == false);

    if (stricmp(userinput, "no") == 0 || stricmp(userinput, "n") == 0){
        printf("Cancelling...\n");
        return;
    }

    // Modifying student entry in DB.
    printf("Modifying student entry in DB...\n");
    if (modifyEntryToDB(student) == false){
        printf("Error: Failed to modify entry with studentind \"%d\".\n", studentind);
        printf("Exiting...\n");
        return;
    }
    printf("Student entry modified.\n");
    return;
}

/**
 * Function to delete an existing student entry from the database.
 * Allows the user to select a student by index number and removes the corresponding entry.
 */
void deleteStudentEntry(){
    // Gathering information on the entry to remove.
    printf("%s", SEPARATOR);
    char userinput[INPUT_BUFFER_LENGHT] = "\0";
    bool entry_found = false;
    int studentind = 0;

    // Loop until a valid student index number is provided.
    while (entry_found == false){
        fgetsStringWhileLoopAlphanumerical("Enter index number of student entry to remove. (Leftmost column in DB).\n",
                                           "Please enter a valid student index number.\n", userinput, DEFAULT_STRING_LENGHT);
        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }
        printf("\n\n%s\n\n", userinput);
        entry_found = stringToIntConv(userinput, &studentind);
    }

    // Fetching student data based on the index number.
    struct Student student = fetchStudentData(studentind); // student.db_entry_row is used to locate entry in DB.
    if (student.fetchFailure == 1){
        return;
    }

    FILE *pFile = openFileWithRetry(DB, "r", 3);
    if (pFile == NULL){
        return;
    }
    FILE *tmpFile = openFileWithRetry(TEMP, "w", 3);
    if (tmpFile == NULL){
        fclose(pFile);
        return;
    }

    // Copying existing data to the temp file without copying the entry to remove and updating the DB row count.
    int current_index = 0, db_rows = 0;
    fscanf(pFile, "%d %d", &current_index, &db_rows);
    fprintf(tmpFile, "%d %d", current_index, db_rows - 1);

    entry_found = false;
    char buffer[LONG_STRING_LENGHT] = "\0";
    while (fgets(buffer, LONG_STRING_LENGHT, pFile) != NULL){
        buffer[strcspn(buffer, "\n")] = '\0';
        if (getIndNum(buffer) != studentind){
            fprintf(tmpFile, "%s", buffer);
        }
        if (getIndNum(buffer) == studentind){
            entry_found = true;
        }
    }

    fclose(pFile);
    fclose(tmpFile);

    if (entry_found == false){
        fprintf(stderr, "Error: Student record with index %d not found.\n", studentind);
        printf("Cancelling...\n");
        remove(TEMP);
        return;
    }

    updateDatabase(DB, TEMP);

    return;
}

/**
 * Creates a backup and then attempts to update the DB file.
 *
 * @param currentDBFileName - The name of the current database file.
 * @param tempDBFileName - The name of the temporary database file.
 */
void updateDatabase(const char *currentDBFileName, const char *tempDBFileName) {
    // Create a backup of the current database.
    if (rename(currentDBFileName, "db_backup.txt") == -1) {
        fprintf(stderr, "Error: Unable to create a backup of the previous DB.\n");
        printf("Cancelling DB update procedure without DB modification...\n");
        remove(tempDBFileName); // Attempt to remove the temporary file, doesn't matter if this fails.
        return;
    }
    printf("Backup of DB created.\n");

    // Update the database file with the new data.
    if (rename(tempDBFileName, currentDBFileName) == -1) {
        fprintf(stderr, "Error: Unable to update the DB file.\n");
        printf("Attempting to restore the previous DB...\n");

        // Attempt to restore the previous DB if the update fails.
        if (rename("db_backup.txt", currentDBFileName) == -1) {
            fprintf(stderr, "Error: Unable to restore the previous DB.\n");
            fprintf(stderr, "Backup can be found in \"db_backup.txt\" and should be restored manually.\n");
            printf("Cancelling DB update procedure...\n");
            return;
        }
        return;
    }
    printf("DB updated.\n");

    // Remove the backup of the previous database.
    if (remove("db_backup.txt") == -1) {
        fprintf(stderr, "Error: Unable to remove the backup of the previous DB.\n");
        fprintf(stderr, "Backup can be found in \"db_backup.txt\" and can be removed manually.\n");
        return;
    }
}

/**
 * Attempts to open the specified file with the given mode.
 * Retries a maximum of 'maxRetries' times in case of failure.
 *
 * @param fileName - The name of the file to open.
 * @param mode - The mode in which to open the file (e.g., "r" for read, "w" for write).
 * @param maxRetries - The maximum number of retry attempts in case of failure.
 *
 * @return A pointer to the opened file if successful.
 *         NULL if unable to open the file after the specified number of retries.
 */
FILE *openFileWithRetry(const char *fileName, const char *mode, const int maxRetries){
    if (fileName == NULL || mode == NULL){
        fprintf(stderr, "Error: Invalid pointer in openFileWithRetry.\n");
        return NULL;
    }
    if (maxRetries < 1){
        fprintf(stderr, "Error: Invalid maxRetries in openFileWithRetry.\n");
        return NULL;
    }

    FILE *file = NULL;
    int retryCount = 0;

    // Attempt to open the file 'maxRetries' times.
    while (retryCount < maxRetries){
        file = fopen(fileName, mode);
        if (file != NULL){
            return file; // File opened successfully.
        }
        retryCount++;
    }

    // Unable to open the file after maximum retries.
    fprintf(stderr, "Error: Unable to open the file \"%s\".\n", fileName);
    return NULL;
}

/**
 * Allows the user to choose between two options: lookup student or browse students.
 * Performs input validation to ensure a valid choice within the specified range.
 * Calls the corresponding function based on the user's choice.
 */
void lookupOrBrowse(){
    char userinput[INPUT_BUFFER_LENGHT] = "\0";
    int userinput_int = 0;
    bool input_valid = false;

    printf("%s", SEPARATOR);

    // Validate user input for the chosen option.
    while (input_valid == false){
        fgetsStringWhileLoopAlphanumerical("1. Lookup student\n2. Browse students\nChoose an option.\n",
                                           "Enter a valid integer between range 1-2.\n",
                                           userinput, DEFAULT_STRING_LENGHT);

        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }

        // Convert the user input to an integer and check if it's within the valid range.
        stringToIntConv(userinput, &userinput_int);
        if (userinput_int >= 1 && userinput_int <= 2){
            input_valid = true;
        }
    }

    // Perform the chosen action based on the user's input.
    switch (userinput_int){
    case 1:
        lookupStudent();
        break;
    case 2:
        browseStudentList();
        break;
    }
    return;
}

/**
 * Allows the user to browse the list of student entries in the database.
 * Asks the user how many rows to show at a time and validates the input.
 * Prints the specified number of rows, showing entries in a tabular format.
 * Prompts the user to continue or exit after each batch of rows.
 */
void browseStudentList(){
    char userinput[INPUT_BUFFER_LENGHT] = "\0";
    int numOfRowsToPrint = 0;
    bool input_valid = false;

    printf("%s", SEPARATOR);

    // Validate user input for the number of rows to print at a time.
    do{
        numOfRowsToPrint = 0;

        // Getting user input for the number of rows to show.
        fgetsStringWhileLoopAlphanumerical("\nHow many rows to show at a time?\nEnter a positive integer.\n",
                                           "Enter a valid integer.\n",
                                           userinput, DEFAULT_STRING_LENGHT);

        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }

        input_valid = stringToIntConv(userinput, &numOfRowsToPrint);  // stringToIntConv wont allow negative numbers.
        if (input_valid == false){
            printf("Error: Enter a valid integer.\n");
        }
    } while (input_valid == false);

    FILE *pFile = openFileWithRetry(DB, "r", 3);
    if (pFile == NULL){
        return;
    }

    // Fetch the number of rows in the DB.
    int studentind = 0, DBrows = 0;
    if (fscanf(pFile, "%d %d", &studentind, &DBrows) != 2){
        fprintf(stderr, "Error: Failed to read DB in browseStudentList.\n");
        fclose(pFile);
        return;
    }

    // Skip first two rows of the DB.
    char columnNames[LONG_STRING_LENGHT] = "\0";
    fgets(columnNames, LONG_STRING_LENGHT, pFile);
    fgets(columnNames, LONG_STRING_LENGHT, pFile);

    // The following loop reads entries from the DB and prints the in structured columns for ease of reading.
    int rowCount = 1, columnCount = 1;
    char buffer[LONG_STRING_LENGHT] = "\0";
    userinput[0] = '\0';
    char *token;
    int columnWidth = 20;
    while (rowCount <= DBrows){
        printf("%s", SEPARATOR);
        printf("Entries %d - %d of %d\n", rowCount, rowCount + numOfRowsToPrint - 1, DBrows);
        printf("%-*s%-*s%-*s%-*s%-*s%-*s\n", 20, "Index", 20, "Studentind", 20, "Firstname", 20, "Lastname", 20, "Studentid", 20, "Major");

        for (int i = 0; i < numOfRowsToPrint; i++){
            columnCount = 1;
            fgets(buffer, LONG_STRING_LENGHT, pFile);
            buffer[strcspn(buffer, "\n")] = '\0';

            token = strtok(buffer, ",");
            printf("%-*d", columnWidth, rowCount);
            while (token != NULL && columnCount < DB_COLUMN_COUNT){
                printf("%-*s", columnWidth, token);
                token = strtok(NULL, ",");
                columnCount++;
            }

            printf("%-*s\n", columnWidth, token);

            rowCount++;
            if (rowCount == DBrows + 1){
                break;
            }
        }

        // Prompt the user to continue or exit.
        printf("Continue? [yes/no]\nInput: ");
        do
        {
            improvedFgets(userinput, DEFAULT_STRING_LENGHT);
            convertToLowercase(userinput);
        } while (stringIsYesOrNo(userinput) == false);

        if (stricmp(userinput, "no") == 0 || stricmp(userinput, "n") == 0){
            printf("Cancelling...\n");
            fclose(pFile);
            return;
        }
    }

    fclose(pFile);
    return;
}

/**
 * Allows the user to look up a specific student entry in the database by entering the student's index number.
 * Validates the user's input for the index number.
 * Retrieves and displays the student's information (firstname, lastname, student ID, major) if found in the database.
 */
void lookupStudent(){
    char userinput[INPUT_BUFFER_LENGHT] = "\0";
    int studentind = 0;
    bool input_valid = false;

    // Asking for student index number.
    printf("%s", SEPARATOR);
    while (input_valid == false){
        fgetsStringWhileLoopAlphanumerical("Enter student index number (Leftmost column in DB).\n",
                                           "Please enter a valid student index number.\n", userinput, DEFAULT_STRING_LENGHT);
        if (exitToCancel(userinput, strlen(userinput)) == true){
            return;
        }
        input_valid = stringToIntConv(userinput, &studentind); // Negative values can pass as the lookup will simply fail to find the entry and prompt the user to retry.
    }

    struct Student student = fetchStudentData(studentind);
    if (student.fetchFailure == 1){ 
        return;
    }

    printf("%s", SEPARATOR);
    printf("Student information\n"
           "Firstname: %s\n"
           "Lastname: %s\n"
           "Studentid: %s\n"
           "Major: %s\n",
           student.firstname, student.lastname, student.studentid, student.major);

    //#TODO: Might be a good idea to put somesort of prompt here to let the user continue,
    //otherwise the program will just return to main() and print the menu in addition to the student information.
    return;
}

/**
 * Tests if a given string represents a "yes" or "no" response.
 * Returns true if the string matches "yes," "no," "y," or "n" (case-insensitive), indicating a valid response.
 * Returns false otherwise.
 */
bool stringIsYesOrNo(const char *str){
    if (str == NULL){
        fprintf(stderr, "Error: Invalid pointer in stringIsYesOrNo.\n");
        return false;
    }

    if (stricmp(str, "yes") == 0 || stricmp(str, "no") == 0 || stricmp(str, "y") == 0 || stricmp(str, "n") == 0){
        return true;
    }
    else{
        printf("Error: Please enter \"yes\" or \"no\".\nInput: ");
        return false;
    }
}
