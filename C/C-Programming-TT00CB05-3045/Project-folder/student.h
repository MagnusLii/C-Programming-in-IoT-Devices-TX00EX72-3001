#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define STR_LEN 256

struct Student
{
    int studentind, db_entry_row;
    char firstname[STR_LEN];
    char lastname[STR_LEN];
    char studentid[STR_LEN];
    char major[STR_LEN];
    int fetchFailure;
};

typedef struct Student Student;

#endif
