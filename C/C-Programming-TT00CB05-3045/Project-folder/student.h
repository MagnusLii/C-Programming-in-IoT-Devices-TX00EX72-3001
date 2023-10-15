#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define STUDENT_ARR_LEN 256

struct Student
{
    int studentind, db_entry_row;
    char firstname[STUDENT_ARR_LEN];
    char lastname[STUDENT_ARR_LEN];
    char studentid[STUDENT_ARR_LEN];
    char major[STUDENT_ARR_LEN];
    int fetchFailure;
};

typedef struct Student Student;

#endif
