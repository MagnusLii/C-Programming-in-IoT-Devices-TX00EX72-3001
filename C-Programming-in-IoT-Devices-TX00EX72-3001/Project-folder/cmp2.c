Student fetchStudentData(const int studentind) {
    Student student = {0};
    char buffer[LONG_STRING_LENGTH];
    FILE *dbFile = openFileWithRetry(DB, "r", 3);

    student.fetchFailure = 0; // Set fetchFailure flag to 0 to indicate no failure.

    if (studentind < 0) {
        fprintf(stdout, "Error: Invalid student index number.\n");
        printf("Cancelling...");
        student.fetchFailure = 1; // Set flag to indicate a failure.
        return student;
    }

    if (dbFile == NULL) {
        fprintf(stdout, "Error: Unable to open file %s", DB);
        printf("Cancelling...");
        student.fetchFailure = 1; // Set flag to indicate a failure.
        return student;
    }

    bool entry_found = false; // Flag to indicate if the entry is found in the database.
    int tokencount = 0;
    int linecount = 0;
    char *token;

    while (fgets(buffer, LONG_STRING_LENGTH, dbFile) != NULL && !entry_found) {
        linecount++;
        int indNum = getIndNum(buffer);
        if (indNum == studentind) {
            token = strtok(buffer, ",");
            while (token != NULL) {
                switch (tokencount) {
                case 0:
                    printf("Token len: %zu\n", strlen(token));
                    printf("Token: %s\n", token);
                    stringToIntConv(token, &student.studentind);
                    break;
                case 1:
                    printf("Token len: %zu\n", strlen(token));
                    printf("Token: %s\n", token);
                    strncpy(student.firstname, token, STUDEN_ARR_LEN - 1);
                    student.firstname[STUDEN_ARR_LEN - 1] = '\0';
                    break;
                case 2:
                    printf("Token len: %zu\n", strlen(token));
                    printf("Token: %s\n", token);
                    strncpy(student.lastname, token, STUDEN_ARR_LEN - 1);
                    student.lastname[STUDEN_ARR_LEN - 1] = '\0';
                    break;
                case 3:
                    printf("Token len: %zu\n", strlen(token));
                    printf("Token: %s\n", token);
                    strncpy(student.studentid, token, STUDENT_ID_LENGHT - 1);
                    student.studentid[STUDENT_ID_LENGHT - 1] = '\0';
                    break;
                case 4:
                    printf("Token len: %zu\n", strlen(token));
                    printf("Token: %s\n", token);
                    strncpy(student.major, token, STUDEN_ARR_LEN - 1);
                    student.major[STUDEN_ARR_LEN - 1] = '\0';
                    break;
                }

                token = strtok(NULL, ",");
                tokencount++;
            }
            student.db_entry_row = linecount; // Set the row number in the database where the entry was found.
            entry_found = true;
        }
    }
    fclose(dbFile);

    if (!entry_found) {
        fprintf(stdout, "Error: Student record with index %d not found.\n", studentind);
        printf("Cancelling...");
        student.fetchFailure = 1; // Set fetchFailure flag to indicate a failure.
    }

    return student;
}
