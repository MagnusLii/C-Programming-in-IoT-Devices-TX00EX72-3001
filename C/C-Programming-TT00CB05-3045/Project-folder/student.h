#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define STUDEN_ARR_LEN 256

struct Student
{
    int studentind, db_entry_row;
    char firstname[STUDEN_ARR_LEN];
    char lastname[STUDEN_ARR_LEN];
    char studentid[STUDEN_ARR_LEN];
    char major[STUDEN_ARR_LEN];
    int fetchFailure;
};

typedef struct Student Student;

#endif
